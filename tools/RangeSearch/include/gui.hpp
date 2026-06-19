#pragma once
#include "RangeSearch.hpp"
#include "kdtree/kdtree.hpp"
#include "rangetree/rangetree.hpp"

#include <boost/geometry.hpp>
#include <boost/geometry/index/rtree.hpp>

#include <array>
#include <cstdint>
#include <utility>
#include <vector>
#include <limits>
#include <sstream>
#include <functional>

// Qt
#include <QComboBox>
#include <QFileDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QThread>
#include <QDialog>
#include <QProgressDialog>
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QFileInfo>

namespace rsgui {

enum class Algorithm { KdTree, RangeTree, BoostRTree };
enum class DataType { Double, Float, Int, Short };

inline constexpr std::size_t kMaxDim = 10;

using RawPoints = std::vector<std::vector<double>>;
using RawRange  = std::vector<std::pair<double, double>>;

template <typename T, std::size_t K>
using KdTree = kdtree::KdTree<T, K>;

template <typename T, std::size_t K>
using Region = kdtree::Region<T, K>;

template <typename T, std::size_t D>
using RangeTreeNd = rangetree::RangeTreeNd<T, D>;

// ---------- boost::rtree helpers ----------
template <typename T, std::size_t K, std::size_t... Is>
void setCoords(boost::geometry::model::point<T, K, boost::geometry::cs::cartesian>& bp,
               const Point<T,K>& p, std::index_sequence<Is...>) {
    (boost::geometry::set<Is>(bp, p[Is]), ...);
}

template <typename T, std::size_t K, std::size_t... Is>
void getCoords(const boost::geometry::model::point<T, K, boost::geometry::cs::cartesian>& bp,
               Point<T,K>& p, std::index_sequence<Is...>) {
    ((p[Is] = static_cast<T>(boost::geometry::get<Is>(bp))), ...);
}

template <typename T, std::size_t K>
std::vector<Point<T,K>> boostRTreeSearch(const std::vector<Point<T,K>>& points,
                                          const Point<T,K>& st, const Point<T,K>& en) {
    namespace bg  = boost::geometry;
    namespace bgi = boost::geometry::index;
    using BPoint = bg::model::point<T, K, bg::cs::cartesian>;
    using BBox   = bg::model::box<BPoint>;

    bgi::rtree<BPoint, bgi::rstar<16>> tree;
    for (const auto& p : points) {
        BPoint bp;
        setCoords<T,K>(bp, p, std::make_index_sequence<K>{});
        tree.insert(bp);
    }

    BPoint minP, maxP;
    setCoords<T,K>(minP, st, std::make_index_sequence<K>{});
    setCoords<T,K>(maxP, en, std::make_index_sequence<K>{});
    BBox box(minP, maxP);

    std::vector<BPoint> found;
    tree.query(bgi::intersects(box), std::back_inserter(found));

    std::vector<Point<T,K>> res;
    res.reserve(found.size());
    for (const auto& bp : found) {
        Point<T,K> p;
        getCoords<T,K>(bp, p, std::make_index_sequence<K>{});
        res.push_back(p);
    }
    return res;
}

// ---------- generic runner ----------
template <typename T, std::size_t K>
RawPoints runSearch(Algorithm alg, const RawPoints& rawPoints, const RawRange& rawRange) {
    std::vector<Point<T,K>> points;
    points.reserve(rawPoints.size());
    for (const auto& rp : rawPoints) {
        Point<T,K> p;
        for (std::size_t i = 0; i < K; ++i) p[i] = static_cast<T>(rp[i]);
        points.push_back(p);
    }

    Point<T,K> st, en;
    for (std::size_t i = 0; i < K; ++i) {
        st[i] = static_cast<T>(rawRange[i].first);
        en[i] = static_cast<T>(rawRange[i].second);
    }

    std::vector<Point<T,K>> result;
    switch (alg) {
        case Algorithm::KdTree: {
            KdTree<T,K> tree(points);
            result = tree.search(Region<T,K>(st, en));
            break;
        }
        case Algorithm::RangeTree: {
            RangeTreeNd<T,K> tree(points);
            result = tree.search(Range<T,K>(st, en));
            break;
        }
        case Algorithm::BoostRTree: {
            result = boostRTreeSearch<T,K>(points, st, en);
            break;
        }
    }

    RawPoints out;
    out.reserve(result.size());
    for (const auto& p : result) {
        std::vector<double> v(K);
        for (std::size_t i = 0; i < K; ++i) v[i] = static_cast<double>(p[i]);
        out.push_back(std::move(v));
    }
    return out;
}

// ---------- runtime dispatch ----------
using RunFn = RawPoints(*)(Algorithm, const RawPoints&, const RawRange&);

template <typename T, std::size_t... Ks>
constexpr std::array<RunFn, sizeof...(Ks)> makeTable(std::index_sequence<Ks...>) {
    return { &runSearch<T, Ks + 1>... };
}

template <typename T>
RawPoints dispatchK(std::size_t K, Algorithm alg, const RawPoints& pts, const RawRange& rng) {
    static constexpr auto table = makeTable<T>(std::make_index_sequence<kMaxDim>{});
    if (K == 0 || K > kMaxDim) throw std::out_of_range("dimension out of supported range");
    return table[K - 1](alg, pts, rng);
}

inline RawPoints dispatch(DataType type, std::size_t K, Algorithm alg,
                           const RawPoints& pts, const RawRange& rng) {
    switch (type) {
        case DataType::Double: return dispatchK<double>(K, alg, pts, rng);
        case DataType::Float:  return dispatchK<float>(K, alg, pts, rng);
        case DataType::Int:    return dispatchK<int>(K, alg, pts, rng);
        case DataType::Short:  return dispatchK<short>(K, alg, pts, rng);
    }
    throw std::logic_error("unknown data type");
}

// ---------- GUI ----------
class RangeSearchGUI : public QMainWindow {
    Q_OBJECT

public:
    explicit RangeSearchGUI(QWidget* parent = nullptr);
    ~RangeSearchGUI() override;  // виртуальный деструктор

private slots:
    void loadFile();
    void runSearch();
    void runBenchmarks();
    void updateTable();

private:
    void setupUI();
    RawPoints parsePointsFile(const QString& filename);
    RawRange getRangeFromTable(std::size_t dim);

    QString m_pointsFile;
    QLabel* m_fileLabel = nullptr;
    QSpinBox* m_dimSpin = nullptr;
    QComboBox* m_algoCombo = nullptr;
    QComboBox* m_typeCombo = nullptr;
    QTableWidget* m_rangeTable = nullptr;
    QLabel* m_resultLabel = nullptr;
};

} // namespace rsgui
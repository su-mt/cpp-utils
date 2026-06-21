#include "gui.hpp"
#include <QProgressBar>  

namespace rsgui {

RangeSearchGUI::RangeSearchGUI(QWidget* parent)
    : QMainWindow(parent) {
    setupUI();
}

RangeSearchGUI::~RangeSearchGUI() = default;

void RangeSearchGUI::loadFile() {
    QString filename = QFileDialog::getOpenFileName(
        this, "Выберите файл с точками", "", "Text Files (.txt);;All Files ()");
    
    if (filename.isEmpty()) return;
    
    m_pointsFile = filename;
    m_fileLabel->setText("Файл: " + QFileInfo(filename).fileName());
    
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        QString line = in.readLine().trimmed();
        QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (!parts.isEmpty()) {
            m_dimSpin->setValue(parts.size());
        }
        file.close();
    }
}

void RangeSearchGUI::runSearch() {
    if (m_pointsFile.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Сначала загрузите файл с точками!");
        return;
    }
    
    RawPoints rawPoints = parsePointsFile(m_pointsFile);
    if (rawPoints.empty()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось прочитать точки!");
        return;
    }
    
    std::size_t dim = m_dimSpin->value();
    if (rawPoints[0].size() != dim) {
        QMessageBox::warning(this, "Ошибка", 
            QString("Размерность не совпадает! Файл: %1, выбрано: %2")
            .arg(rawPoints[0].size()).arg(dim));
        return;
    }
    
    RawRange range = getRangeFromTable(dim);
    Algorithm alg = static_cast<Algorithm>(m_algoCombo->currentIndex());
    DataType type = static_cast<DataType>(m_typeCombo->currentIndex());
    
    try {
        RawPoints results = dispatch(type, dim, alg, rawPoints, range);QString algoName = m_algoCombo->currentText();
        QString typeName = m_typeCombo->currentText();
        
        QString html = QString("<b>%1 &lt;%2&gt;</b>: найдено %3 из %4 точек<br><br>")
            .arg(algoName).arg(typeName)
            .arg(results.size()).arg(rawPoints.size());
        
        int show = std::min(10, static_cast<int>(results.size()));
        for (int i = 0; i < show; ++i) {
            html += "(" + QString::number(results[i][0], 'f', 2);
            for (size_t j = 1; j < results[i].size(); ++j) {
                html += ", " + QString::number(results[i][j], 'f', 2);
            }
            html += ")<br>";
        }
        if (results.size() > 10) {
            html += QString("... и ещё %1").arg(results.size() - 10);
        }
        
        m_resultLabel->setText(html);
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", e.what());
    }
}

void RangeSearchGUI::runBenchmarks() {
    QDialog* dlg = new QDialog(this);
    dlg->setWindowTitle("Бенчмарки");
    dlg->setModal(true);
    QVBoxLayout* l = new QVBoxLayout(dlg);
    QLabel* label = new QLabel("Запуск бенчмарков...", dlg);
    QProgressBar* bar = new QProgressBar(dlg);
    bar->setRange(0, 0);
    l->addWidget(label);
    l->addWidget(bar);
    dlg->setLayout(l);
    dlg->setMinimumWidth(300);
    dlg->show();
    QCoreApplication::processEvents();
    
    QThread* t = QThread::create([]() {
        QString dir = "build/src/RangeSearch";
        system(QString("cd %1 && python3 -m venv venv 2>/dev/null").arg(dir).toStdString().c_str());
        system(QString("cd %1 && source venv/bin/activate && pip install matplotlib scipy numpy -q 2>/dev/null").arg(dir).toStdString().c_str());
        system(QString("cd %1 && ./build/benchmarks/run_benchmarks --benchmark_format=json --benchmark_out=benchmark_results.json --findvary").arg(dir).toStdString().c_str());
        system(QString("cd %1 && source venv/bin/activate && python analysis/analyze.py").arg(dir).toStdString().c_str());
    });
    
    connect(t, &QThread::finished, this, [this, dlg]() {
        dlg->close();
        delete dlg;
        QMessageBox::information(this, "Готово", "Графики сохранены в папке analysis/plots");
    });
    
    t->start();
}

void RangeSearchGUI::updateTable() {
    int dim = m_dimSpin->value();
    m_rangeTable->setRowCount(dim);
    
    for (int i = 0; i < dim; ++i) {
        QTableWidgetItem* label = new QTableWidgetItem(QString::number(i));
        m_rangeTable->setVerticalHeaderItem(i, label);
        
        for (int j = 0; j < 2; ++j) {
            if (!m_rangeTable->item(i, j)) {
                m_rangeTable->setItem(i, j, new QTableWidgetItem("0"));
            }
        }
    }
}

void RangeSearchGUI::setupUI() {
    setWindowTitle("RangeSearch GUI");
    resize(550, 600);
    
    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout* v = new QVBoxLayout(central);
    
    // File
    QGroupBox* fileGroup = new QGroupBox("Файл с точками", central);
    QHBoxLayout* fileL = new QHBoxLayout(fileGroup);
    QPushButton* loadBtn = new QPushButton("Загрузить", fileGroup);
    m_fileLabel = new QLabel("Файл: не выбран", fileGroup);
    fileL->addWidget(loadBtn);
    fileL->addWidget(m_fileLabel);
    connect(loadBtn, &QPushButton::clicked, this, &RangeSearchGUI::loadFile);
    v->addWidget(fileGroup);
    
    // Params
    QGroupBox* paramGroup = new QGroupBox("Параметры", central);
    QGridLayout* pL = new QGridLayout(paramGroup);
    
    pL->addWidget(new QLabel("Размерность:"), 0, 0);
    m_dimSpin = new QSpinBox(paramGroup);
    m_dimSpin->setRange(1, kMaxDim);
    m_dimSpin->setValue(2);
    connect(m_dimSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &RangeSearchGUI::updateTable);
    pL->addWidget(m_dimSpin, 0, 1);
    
    pL->addWidget(new QLabel("Алгоритм:"), 1, 0);
    m_algoCombo = new QComboBox(paramGroup);
    m_algoCombo->addItems({"KdTree", "RangeTree", "BoostRTree"});
    pL->addWidget(m_algoCombo, 1, 1);
    
    pL->addWidget(new QLabel("Тип данных:"), 2, 0);
    m_typeCombo = new QComboBox(paramGroup);
    m_typeCombo->addItems({"double", "float", "int", "short"});
    pL->addWidget(m_typeCombo, 2, 1);
    
    v->addWidget(paramGroup);
    
    // Range table
    QGroupBox* rangeGroup = new QGroupBox("Search Range", central);
    QVBoxLayout* rL = new QVBoxLayout(rangeGroup);
    m_rangeTable = new QTableWidget(rangeGroup);
    m_rangeTable->setColumnCount(2);
    m_rangeTable->setHorizontalHeaderLabels({"start", "end"});
    m_rangeTable->setColumnWidth(0, 120);
    m_rangeTable->setColumnWidth(1, 120);
    m_rangeTable->setMinimumHeight(200);
    rL->addWidget(m_rangeTable);
    v->addWidget(rangeGroup);
    updateTable();
    
    // Buttons
    QHBoxLayout* btnL = new QHBoxLayout();
    QPushButton* searchBtn = new QPushButton("Поиск");
    QPushButton* graphBtn = new QPushButton("Графики");
    searchBtn->setMinimumHeight(45);
    graphBtn->setMinimumHeight(45);
    connect(searchBtn, &QPushButton::clicked, this, &RangeSearchGUI::runSearch);
    connect(graphBtn, &QPushButton::clicked, this, &RangeSearchGUI::runBenchmarks);
    btnL->addWidget(searchBtn);
    btnL->addWidget(graphBtn);
    v->addLayout(btnL);
    
    // Result
    m_resultLabel = new QLabel("Результат здесь", central);
    m_resultLabel->setWordWrap(true);
    m_resultLabel->setStyleSheet("padding:10px; background:#f5f5f5; border:1px solid #ccc;");
    v->addWidget(m_resultLabel);
}

RawPoints RangeSearchGUI::parsePointsFile(const QString& filename) {
    RawPoints pts;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return pts;
    
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        
        QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        std::vector<double> p;
        for (const QString& s : parts) {
            bool ok;
            double v = s.toDouble(&ok);
            if (ok) p.push_back(v);
        }
        if (!p.empty()) pts.push_back(std::move(p));
    }
    return pts;
}

RawRange RangeSearchGUI::getRangeFromTable(std::size_t dim) {
    RawRange rng;
    for (std::size_t i = 0; i < dim; ++i) {
        double st = m_rangeTable->item(i, 0)->text().toDouble();
        double en = m_rangeTable->item(i, 1)->text().toDouble();
        rng.push_back({st, en});
    }
    return rng;
}

} // namespace rsgui
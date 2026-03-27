import re
import os
import sys
import time
from dataclasses import dataclass
from typing import List, Dict

@dataclass
class CarActivity:
    id: int
    sector: int
    start_t: int
    end_t: int

# ---------------- STATE & DATA ----------------

car_speeds = {}
car_routes = {}
# Список всех "пребываний" машин в секторах
activities: List[CarActivity] = []
# Машины на дорогах (вне перекрестка)
roads = {"North": [], "South": [], "East": [], "West": []}

# ---------------- PARSE ----------------

def parse_log(filename):
    with open(filename) as f:
        for line in f:
            # 1. Запоминаем скорость
            m = re.search(r'\[(\d+)\] Created Car#(\d+) road=(\w+).*speed=(\d+)', line)
            if m:
                t, cid, road, speed = int(m.group(1)), int(m.group(2)), m.group(3), int(m.group(4))
                car_speeds[cid] = speed
                roads[road].append((t, cid)) # Запоминаем время появления на дороге
                continue

            # 2. Запоминаем маршрут
            m = re.search(r'Car#(\d+) attempting route \[([\d,]+)\]', line)
            if m:
                cid, route = int(m.group(1)), [int(x) for x in m.group(2).split(',')]
                car_routes[cid] = route
                continue

            # 3. Вход в сектор (accepted или entered)
            # 3. Вход в сектор
            m_ent = re.search(r'\[(\d+)\] Car#(\d+) entered sector (\d+)', line)
            m_left = re.search(r'\[(\d+)\] Car#(\d+) left sector (\d+)', line)

            if m_ent:
                t = int(m_ent.group(1))
                cid = int(m_ent.group(2))
                sec = int(m_ent.group(3))

                # временно сохраняем начало
                activities.append(CarActivity(cid, sec, t, None))

            elif m_left:
                t = int(m_left.group(1))
                cid = int(m_left.group(2))
                sec = int(m_left.group(3))

                # находим последнее незакрытое пребывание
                for a in reversed(activities):
                    if a.id == cid and a.sector == sec and a.end_t is None:
                        a.end_t = t
                        break

# ---------------- RENDER ----------------

def draw(current_t, current_sectors, current_roads):
    os.system('cls' if os.name == 'nt' else 'clear')

    def get_road_cars(road_name):
        # Показываем только тех, кто уже "создан", но еще не на перекрестке
        count = len(current_roads[road_name])
        return "x" * count

    def cell(sid):
        ids = current_sectors.get(sid, [])
        if not ids: return " "
        return "*" # Символ машины на перекрестке

    print(f"Time: {current_t:08d}")
    print("      (North)")
    
    # North road (вертикально)
    n_count = len(current_roads["North"])
    for i in range(3):
        char = "x" if i < n_count else " "
        print(f"                {char}")
    
    print("          |    |    |")
    
    # Центр
    west_road = get_road_cars("West").rjust(10)
    east_road = get_road_cars("East").ljust(10)
    
    print("----------+----+----+----------")
    print(f"{west_road}| {cell(2)}  | {cell(1)}  |{east_road}")
    print("----------|----+----|----------")
    print(f"{west_road}| {cell(3)}  | {cell(0)}  |{east_road}")
    print("----------+----+----+----------")

    print("          |    |    |")
    
    # South road
    s_count = len(current_roads["South"])
    for i in range(3):
        char = "x" if i < s_count else " "
        print(f"                {char}")
    print("      (South)")

# ---------------- MAIN ----------------

def main():
    if len(sys.argv) < 2:
        print("usage: python sim.py log.txt")
        return

    parse_log(sys.argv[1])
    
    if not activities: return
    # удалить незавершенные активности
    activities[:] = [a for a in activities if a.end_t is not None]

    # Находим временные границы
    max_t = max(a.end_t for a in activities)
    
    # Шаг симуляции (чем меньше, тем плавнее)
    step = 100 
    
    for t in range(0, max_t + step, step):
        # 1. Фильтруем машины в секторах на данный момент t
        current_sectors = {0: [], 1: [], 2: [], 3: []}
        active_ids = set()
        for a in activities:
            if a.start_t <= t < a.end_t:
                current_sectors[a.sector].append(a.id)
                active_ids.add(a.id)
        
        # 2. Фильтруем машины на дорогах (кто создан, но еще не зашел в сектор)
        current_roads = {"North": [], "South": [], "East": [], "West": []}
        for r_name, r_list in roads.items():
            for spawn_t, cid in r_list:
                # Если машина уже создана, но еще не появилась ни в одном секторе симуляции
                # Ищем первое появление этой машины в активностях

                first_entry = min(
                    [a.start_t for a in activities if a.id == cid],
                    default=None
                )

                if spawn_t <= t and (first_entry is None or t < first_entry):
                    current_roads[r_name].append(cid)

        draw(t, current_sectors, current_roads)
        time.sleep(0.1)

if __name__ == "__main__":
    main()
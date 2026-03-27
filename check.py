import re
import sys

def check_log(filename):
    events = []

    with open(filename) as f:
        for line in f:
            m_ent = re.search(r'\[(\d+)\] Car#(\d+) entered sector (\d+)', line)
            m_left = re.search(r'\[(\d+)\] Car#(\d+) left sector (\d+)', line)

            if m_ent:
                t = int(m_ent.group(1))
                cid = int(m_ent.group(2))
                sec = int(m_ent.group(3))
                events.append((t, 'enter', cid, sec))

            elif m_left:
                t = int(m_left.group(1))
                cid = int(m_left.group(2))
                sec = int(m_left.group(3))
                events.append((t, 'leave', cid, sec))

    # сортировка по времени (важно!)
    events.sort(key=lambda x: (x[0], 0 if x[1] == 'leave' else 1))

    # текущее состояние секторов
    sectors = {0: set(), 1: set(), 2: set(), 3: set()}

    for t, typ, cid, sec in events:
        if typ == 'enter':
            if sectors[sec]:
                print(f"[ERROR] Conflict at time {t}: "
                      f"Car#{cid} enters sector {sec}, "
                      f"but already occupied by {sectors[sec]}")
                return
            sectors[sec].add(cid)

        elif typ == 'leave':
            sectors[sec].discard(cid)

    print("OK: no conflicts detected")


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("usage: python check.py crossroads.log")
    else:
        check_log(sys.argv[1])
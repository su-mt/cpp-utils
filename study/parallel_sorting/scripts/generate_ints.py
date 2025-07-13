import random

def generate_ints(filename: str, count: int = 10**6, min_value: int = -10**9, max_value: int = 10**9):
    with open(filename, 'w') as f:
        for _ in range(count):
            value = random.randint(min_value, max_value)
            f.write(f"{value}\n")

if __name__ == "__main__":
    generate_ints("ints.txt")

import random

def generate_floats(filename: str, count: int = 10**6, min_value: float = -1e6, max_value: float = 1e6):
    with open(filename, 'w') as f:
        for _ in range(count):
            value = random.uniform(min_value, max_value)
            f.write(f"{value}\n")

if __name__ == "__main__":
    generate_floats("floats.txt")

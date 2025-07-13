
import random
import string

def generate_random_strings(filename: str, count: int = 5*10**8, min_len: int = 5, max_len: int = 15):
    with open(filename, 'w') as f:
        for _ in range(count):
            length = random.randint(min_len, max_len)
            rand_str = ''.join(random.choices(string.ascii_letters + string.digits, k=length))
            f.write(rand_str + '\n')

if __name__ == "__main__":
    generate_random_strings("big.txt")

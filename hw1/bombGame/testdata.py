import random

n = 100000
m = 100000

print(f"{n} {m}")

for i in range(m):
    if random.random() < 0.5:
        c = random.randint(1, n)
        d = random.randint(1, 10 ** 9)
        print(f"P {c} {d}")
    else:
        left = random.randint(1, n)
        right = random.randint(1, n)
        if left > right:
            left , right = right, left
        p = random.randint(1, 10 ** 9)
        k = random.randint(1, 10 ** 4)
        print(f"A {left} {right} {p} {k}")

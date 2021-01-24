import random

rangeX = (-10 ** 9, 1 + 10 ** 9)

n = 500
m = 500
k = 0
c = 10 ** 9

coords = [random.randint(*rangeX) for _ in range(n * m)]

print(f"{n} {m} {k} {c}")

cnt = 0

for row in range(n):
    for col in range(m - 1):
        print(coords[cnt], end=" ")
        cnt += 1
    print(coords[cnt])
    cnt += 1

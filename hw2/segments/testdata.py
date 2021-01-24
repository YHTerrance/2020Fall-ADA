import random


N = 2 * 10 ** 5
M = 2 * 10 ** 5
W = 3 * 10 ** 13

print(f"{N} {M} {W}")

for i in range(N):
    l = random.randint(1,M - 1)
    r = random.randint(l + 1, M)
    c = random.randint(1, W)

    print(f"{l} {r} {c}")


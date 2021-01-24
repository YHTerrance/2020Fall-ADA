import random


N = 3 * 10 ** 3
M = 3 * 10 ** 3
W = 10 ** 9
S = 1
T = random.randint(2, N)

print(f"{N} {M} {S} {T}")

for i in range(M):
    u = random.randint(1,M - 1)
    v = random.randint(u + 1, M)
    w = random.randint(1, W)

    print(f"{u} {v} {w}")


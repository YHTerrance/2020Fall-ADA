import random

N = 5 * 10 ** 5
K = 5 * 10 ** 5
maxA = 10 ** 9

print(f"{N} {K}")

for i in range(N):
    a = random.randint(1, maxA + 1)
    print(a, end=' ')

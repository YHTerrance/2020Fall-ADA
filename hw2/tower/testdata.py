import random

rangeX = (-10 ** 9, 1 + 10 ** 9)

B = 10 ** 5
N = 10 ** 5
A = 10 ** 9
K = 400

print(f"{N} {K} {A} {B}")

for i in range(N):
    print(random.randint(rangeX[0], rangeX[1]), end=" ")

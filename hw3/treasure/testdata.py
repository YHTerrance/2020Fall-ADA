import random


N = 5 * 10 ** 3
M = 5 * 10 ** 3
treasure = 10 ** 9

print(f"{N} {M}")

cities = []

for i in range(N):
    cities.append(str(random.randint(1, treasure)))

print((" ").join(cities))

for i in range(M):
    u = random.randint(1, N)
    v = random.randint(1, N)

    while u == v:
        v = random.randint(1, N)

    print(f"{u} {v} {random.randint(1, treasure)}")

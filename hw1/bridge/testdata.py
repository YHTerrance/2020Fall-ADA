import random

rangeX = (0, 10 ** 9)

coords = [(random.randint(*rangeX), random.randint(*rangeX)) for _ in range(200000)]

print(len(coords))
for coord in coords:
    print(coord[0], coord[1])

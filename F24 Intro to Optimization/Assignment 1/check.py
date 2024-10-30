from itertools import product

expected = 0
print(5)
all_variants = list(product([1, 2, 3, 4, 5, 6], repeat=120))
print(6)
print(len(all_variants))
for variant in all_variants:
    variant = list(variant)
    cnt = 0
    for i in range(119):
        if variant[i] == 2 and variant[i + 1] == 3:
            cnt += 1
    expected += cnt * (1 / len(all_variants))

print(expected)
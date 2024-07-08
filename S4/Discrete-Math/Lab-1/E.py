from math import gcd
 
r = int(input())
d = int(input())
p = list(map(int, input().split()))
 
def at(a: list, pos: int):
    return a[pos] if pos < len(a) else 0
 
def sum(a: list, b: list):
    mx_k = max(len(a), len(b)) + 1
    c = [0 for _ in range(mx_k)]
 
    k = 0
 
    for i in range(mx_k):
        c[i] = at(a, i) + at(b, i)
 
        if c[i] != 0:
            k = i
 
    return c[:k + 1]
 
def product(a: list, b: list):
    mx_k = len(a) + len(b) + 1
    c = [0 for _ in range(mx_k)]
 
    k = 0
 
    for i in range(mx_k):
        for j in range(i + 1):
            c[i] += at(a, j) * at(b, i - j)
 
        if c[i] != 0:
            k = i
 
    return c[:k + 1]
 
def fact(n: int, start = 1):
    res = 1
    for i in range(start, n + 1):
        res *= i
 
    return res
 
triangle = [[1]]
for i in range(1, d + 1):
    triangle.append(product(triangle[i - 1], [1, i]))
 
for i in range(d + 1):
    p[i] *= fact(d)
 
for i in range(d + 1):
    for j in range(i + 1):
        triangle[i][j] *= fact(d, i + 1)
 
res = [0] * (d + 1)
for i in range(d, -1, -1):
    a = p[i]
    b = 0
 
    for j in range(d - i):
        b += triangle[d - j][d - j - i] * res[d - j]
 
    res[i] = (a - b) // triangle[i][0]
 
q = [1]
tmp = [1, -r]
for i in range(d, -1, -1):
    res[i] = product([res[i]], q)
    q = product(q, tmp)
 
c = [0]
for i in range(d + 1):
    c = sum(c, res[i])
 
print(len(c) - 1)
print(*c)
print(len(q) - 1)
print(*q)

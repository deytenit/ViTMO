from math import gcd, factorial
 
r, k = map(int, input().split())
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
 
triangle_ = [[1, 1]]
for i in range(1, k + 1):
    triangle_.append(product(triangle_[i - 1], triangle_[0]))
 
triangle = [[] for _ in range(len(triangle_[len(triangle_) - 1]))]
for i in range(len(triangle_)):
    for j in range(len(triangle_[i])):
            triangle[j].append(triangle_[i][j])
 
triangle = list(reversed(triangle))
 
q_nmrs = [0 for _ in range(k + 1)]
q_dnmr = r ** k
 
for i in range(k, -1, -1):
    a = (r ** (k - i)) * p[i]
    b = 0
 
    for j in range(i + 1, k + 1):
        b += triangle[k - i + 1][j - i] * q_nmrs[j]
 
    if i % 2 == 0:
        q_nmrs[i] = a - b
    else:
        q_nmrs[i] = -a - b
 
q_dnmr *= fact(k)
 
for i in range(0, k + 1):
    q_nmrs[i] *= fact(k, k - i + 1)
 
res = [q_nmrs[k]]
p = [1, 1]
for i in range(k - 1, -1, -1):
    res = sum(res, product([q_nmrs[i]], p))
    p = product(p, [(k - i + 1), 1])
 
for i in range(k + 1):
    g = gcd(at(res, i), q_dnmr)
    print(f'{at(res, i) // g}/{q_dnmr // g}', end=' ')
print()

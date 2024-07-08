MOD = 104857601
 
k, n = map(int, input().split())
a = list(map(int, input().split()))
c = list(map(int, input().split()))
 
def at(a: list, pos: int):
    return a[pos] if pos < len(a) else 0
 
def product(a: list, b: list, limit = 9223372036854775807):
    mx_k = min(len(a) + len(b) + 1, limit)
    c = [0 for _ in range(mx_k)]
 
    k = 0
 
    for i in range(mx_k):
        for j in range(i + 1):
            c[i] += at(a, j) * at(b, i - j)
 
        if c[i] != 0:
            k = i
 
        c[i] = (c[i] + MOD) % MOD
 
    return c[:k + 1]
 
def prodNeg(a: list, b: list):
    return product(a, [b[i] if i % 2 == 0 else -b[i] for i in range(len(b))])
 
def subst(a: list, offset: int):
    c = []
 
    for i in range(len(a)):
        if (2 * i + offset < len(a)):
            c.append(a[2 * i + offset])
 
    return c
 
q = [1]
for it in c:
    q.append(-it)
 
p = product(a, q, k)
 
n -= 1
 
while n != 0:
    p = prodNeg(p, q)
 
    p = subst(p, n % 2)
 
    q = subst(prodNeg(q, q), 0)
 
    n //= 2
 
print((p[0] + MOD) % MOD)

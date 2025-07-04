import numpy as np

def solve_sle(matrix, coeffs):
    n = len(matrix)
    m = len(matrix[0])

    sle = np.zeros((n, m + 1))
    for i in range(n):
        for j in range(m):
            sle[i, j] = matrix[i][j]
        sle[i, m] = coeffs[i]

    for i in range(n):
        r = i
        for k in range(i + 1, n):
            if abs(sle[k, i]) > abs(sle[r, i]):
                r = k

        if i != r:
            sle[[i, r]] = sle[[r, i]]

        if abs(sle[i, i]) < 1e-9:
            continue

        for k in range(i + 1, n):
            factor = sle[k, i] / sle[i, i]
            for j in range(i, m + 1):
                sle[k, j] -= factor * sle[i, j]

    result = np.zeros(n)

    for i in range(n - 1, -1, -1):
        sum_val = 0.0

        for j in range(i + 1, m):
            sum_val += sle[i, j] * result[j]

        if abs(sle[i, i]) < 1e-9:
            result[i] = 0.0
        else:
            result[i] = (sle[i, m] - sum_val) / sle[i, i]

    return result

def solve():
    n, m = map(int, input().split())
    matrix = []

    for i in range(n):
        row = list(map(float, input().split()))
        matrix.append(row)

    matrix = np.array(matrix)
    result = []

    for i in range(m):
        row = list(map(float, input().split()))
        result.append(row)

    result = np.array(result)
    k = n - m + 1
    a = np.zeros((m * m, k * k))

    for i in range(m * m):
        for j in range(k * k):
            a[i, j] = matrix[(i // m) + (j // k), (i % m) + (j % k)]

    b = np.zeros(m * m)

    for i in range(m * m):
        b[i] = result[i // m, i % m]

    transposed = a.T
    normal = transposed @ a
    rhs = transposed @ b

    solve_result = solve_sle(normal.tolist(), rhs.tolist())

    for i in range(k):
        for j in range(k):
            print(f'{solve_result[i * k + j]:.20f}', end=' ')
        print()

solve()

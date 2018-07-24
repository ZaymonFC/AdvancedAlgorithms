import math

def PrintParenthesis(i, j):
    if i == j:
        print(' M', j, end=' ', sep='')
    else:
        print('(', end='',)
        PrintParenthesis(i, s[i][j])
        PrintParenthesis(s[i][j] + 1, j)
        print(')', end='')

matrices = [40, 20, 30, 10, 30, 60, 100, 10, 5, 10, 50]
n = len(matrices)
m = [[0 for x in range(n)] for y in range(n)]
s = [[0 for x in range(n)] for y in range(n)]

# Calculate the table
for i in range(n, 0, -1):
    for j in range(i + 1, n):
        m[i][j] = math.inf
        for k in range(i, j):
            cost = m[i][k] + m[k + 1][j] + \
                matrices[i - 1] * matrices[k] * matrices[j]
            if cost < m[i][j]:
                m[i][j] = cost
                s[i][j] = k

print('Total number of operations: ', m[1][n-1])
PrintParenthesis(1, n-1)

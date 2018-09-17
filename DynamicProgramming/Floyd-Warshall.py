def FloydWarshal(matrix):
    size = len(matrix)
    for i in range(1, size):
        for j in range(1, size):
            for k in range(1, size):
                matrix[i][j] = min(matrix[i][j], matrix[i][k] + matrix[k][j])
    return matrix

def PrintMatrix(matrix):
    for row in matrix:
        for col in row:
            print(col, '\t', end='')
        print()


d = [[0, 0, 0, 0, 0, 0], [0, 0, 3, 8, 999999, -4],
     [0, 999999, 0, 999999, 1, 7], [0, 999999, 4, 0, 999999, 999999],
     [0, 2, 999999, -5, 0, 999999], [0, 999999, 999999, 999999, 6, 0]]

PrintMatrix(FloydWarshal(d))
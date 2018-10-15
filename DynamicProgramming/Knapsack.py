def KnapSack(knapWt, objWts, objVals):
    n = len(objWts)
    M = [[0 for i in range(knapWt + 1)] for j in range(n+1)]

    for i in range(1, n+1):
        for w in range(1, knapWt+1):
            if objWts[i-1] <= w:
                M[i][w] = max(objVals[i-1] + M[i-1][w - objWts[i-1]], M[i-1][w])
            else:
                M[i][w] = M[i-1][w]
    return M[n][knapWt]

knapWt = 50
objWts = [10, 20, 30]
objVals = [60, 100, 120]

print(KnapSack(knapWt, objWts, objVals))
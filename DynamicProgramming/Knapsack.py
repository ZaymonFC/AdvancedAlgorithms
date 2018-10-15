# Knapsack Problem
def Knapsack(targetWeight, objectValues, objectWeights):
    n = len(objectValues)
    m = [0 for _ in range(targetWeight + 1) for _ in range(n + 1)]

    for i in range(1, n + 1):
        for w in range(1, targetWeight + 1):
            if objectWeights[i - 1] <= w:
                m[i][w] = max(objectValues[i-1] + m[i - 1], w - objectWeights[i - 1]], m[i-1][w])
            else:
                m[i][w] = m[i - 1][w]
    return m[n][targetWeight]


weightLimit = 50
objectWeights = [10, 20, 30]
objValues = [60, 100, 120]

print(Knapsack(weightLimit, objectWeights, objValues))

# Knapsack Problem
def Knapsack(maxWeight, objectWeights, objectValues):
    n = len(objectValues)
    m = [[0 for _ in range(maxWeight + 1)] for _ in range(n + 1)]

    for i in range(n + 1):
        for w in range(maxWeight + 1):
            if objectWeights[i - 1] <= w:
                m[i][j] = max(objectValues[i - 1] + m[i - 1][w - objectWeights[i - 1]], m[i - 1][w])
            else:
                m[i][j] = m[i - 1][w]
    return m[n][maxWeight]

weightLimit = 50
objectWeights = [10, 20, 30]
objValues = [60, 100, 120]

print(Knapsack(weightLimit, objectWeights, objValues))

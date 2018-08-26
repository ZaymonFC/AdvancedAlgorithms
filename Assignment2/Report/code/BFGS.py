FUNCTION BFGS(sequence):
    gradientVector = sequence.CalcGradients


ALGORITHM GA_BFGS(sequenceLength):
    populationSize <- ALGORITHMPARAMETER
    epsilon <- ALGORITHMPARAMETER

    # Mutation chances
    mutation1 <- ALGORITHMPARAMETER
    mutation2 <- ALGORITHMPARAMETER
    mutation3 <- ALGORITHMPARAMETER

    population <- [vector<Sequence>] generatePopulation(sequenceLength)

    lastBest <- infinity
    bestScore <- infinity
    bestSequence <- Sequence

    while true:
        for member in population:
            member <- BFGS(member)
        
        scoredPopulation <- [vector<pair<cost, Sequence>>] sort(population)
        diff <- lastBest - scorePopulation[0].cost
        lastBest <- scoredPopulation[0].cost

        if lastBest <- bestScore:
            bestScore <- lastBest
        
        for member in population:
            if rand > mutation1:
                member.mutateRandomAngles()
                BFGS(member)
            if rand > mutation2:
                member.clearSubsequent()

        for randomPair in population:
            newPairs = crossOver(randomPair)
            randomPair.first = newPairs[0]
            randomPair.last = newPair[1]

        # Check exit conditions
        if (diff < epsilon):
            print bestScore
            break



ALGORITHM CrossEntropy (sequenceLength):
    populationSize <- ALGORITHMPARAMETER
    parameterBlendRatio <- ALGORITHMPARAMETER
    eliteDistributionPercentage <- ALGORITHMPARAMETER
    epsilon <- ALGORITHMPARAMETER

    distributions <- [vector<normal distribution>] with default parameters
    population <- [vector<Sequence>] generatePopulation(populationSize, distributions)
    
    lastBest <- infinity
    bestScore <- infinity
    bestSequence <- Sequence

    while True:
        # Sort Population by Cost
        scoredPopulation <- [vector<pair<cost, Sequence>>] sort(population)
        diff <- lastBest - scorePopulation[0].cost
        lastBest <- scoredPopulation[0].cost

        if lastBest < bestScore:
            bestScore = lastBest
        
        # Update Distributions
        for angle in 0 <= angle < sequenceLength:
            values <- []
            for memberIndex in 0 <= memberIndex < populationSize * eliteDistributionPercentage:
                values <+ scoredPopulation[memberIndex].Sequence[angle]
            
            mean <- mean(values)
            stdDeviation <- stdDeviation(values)

            distributions[angle].updateParameters(mean, stdDeviation)
        
        # Check exit conditions
        if (diff < epsilon):
            if (restartAttemptsRemaining):
                performPartialReset()
            else:
                print bestScore
                break
        
        # Generate new Population
        population = generatePopulation(populationSize, distributions)
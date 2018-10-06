def ConstructVCGreedy(Graph g, QuickSet vSet):
	index <- index between 0 .. |V|
	vSet <-+ index

	while vSet is not vertex cover of g:
		degrees <- g.GetDegreeList(vSet)
		sort degrees
		vSet <-+ highestDegreeNode[degrees]


ALGORITHM MVC_Search(Graph g, int swapLimit, int operationsPerPhase, int iterationCap):
	QuickSet vSet <- QuickSet(g.vertices)
	ConstructVCGreedy(g, vSet)

	penalty <- list of -1, length = |V|
	
	iterations <- 0
	bestCover <- |vSet|

	while iterations < iterationCap:
		for operation < operationsPerPhase:
			depth <- random between 0...10

			// Reduce the number of nodes in the list
			for i in range 0...depth:
				u <- random between 0...|vSet|

				while vSet does not contain u:
					u <- random between 0...|vSet|
				remove u from vSet
				AddToPenalty(penalty, u)
				UpdatePenalty(penalty)

				if vertex cover break
			if vertex cover break

			// Rebuild - Stopping early if we find a better cover
			surface <- 0
			for i in range 0...depth:
				degree <- g.GetDegreeList(vSet)
				sort degree

				index <- 0
				while penalty[degree[index].node]:
					index <- index + 1
					if index <- |degree|:
						index <- random in range 0... |V| - |vSet|
						break

				vSet <-+ degree[index].node
				AddToPenalty(penalty, degree.at(index).second)
				UpdatePenalty(penalty)

				if g is vertex cover break
		if g is vertex cover:
			if (|vSet| < bestCover) bestCover <- |vSet|


		// --- Stabalisation Phase
		if odd iteration:
			v <- random between 0...|vSet|
			vSet.Remove(v)

		swapCount <- 0

		// Get Back to a vertex cover with swaps
		while vSet is not vertex cover of g and swapCount < swapLimit:
			degree <- g.GetDegreeList(vSet)
			sort degree

			index <- 0
			while penalty[degree[index].node]:
				index <- index + 1
				if index <- |degree|:
					index <- random between 0...|degree|
					break
			vSet <-+ degree[index].node

			while true:
				u <- random in range 0...|vSet|
				if penalty.at(u) > 0 continue

			AddToPenalty(penalty, degree.at(index).second)
			UpdatePenalty(penalty)
			swaps <- swaps - 1

		if g is vertex cover:
			if |vSet| < bestCover:
				bestCover <- |vSet|

		if iteration is odd 
			degree <- g.GetDegreeList(vSet)
			sort degree
			vSet <-+ one node from top 5 in degree
		iterations <- iterations + 1
	return bestCover

def SearchRunner(Graph g):
	maximumSwaps <- 100
	operations <- 50
	iterations <- 2
	restarts <- 50

	best <- max int

	for i in range 0...restarts:
		score <- MVC_Search(g, maximumSwaps, operations, iterations)
		if score < best:
			best <- score
	return best

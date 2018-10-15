# | start | finish | weight |
jobs = [[1, 2, 50],
        [3, 5, 20],
        [6, 19, 100],
        [2, 100, 200]]

def LatestCompatible(jobs, job):
    for lastJob in range(job - 1, -1, -1):
        if (jobs[lastJob][1] <= jobs[job][0]):
            return lastJob
    return -1

def FindSolution(jobs, m, job):
    lastJob = LatestCompatible(jobs, job)

    if (job == 0):
        return
    elif (jobs[job][2] + m[lastJob] > m[job - 1]):
        print("Job: ", job, " ", jobs[job])
        if (lastJob == 0):
            print("Job: ", lastJob, " ", jobs[lastJob])
        FindSolution(jobs, m, lastJob)
    else:
        FindSolution(jobs, m, job - 1)

def BestProfit(jobs):
    jobs.sort(key=lambda  x: x[1])

    m = [jobs[0][2]]

    for job in range(1, len(jobs)):
        jobWeight = jobs[job][2]
        lastJob = LatestCompatible(jobs, job)
        if (lastJob != -1):
            jobWeight += m[lastJob]
        m.append(max(jobWeight, m[-1]))
    FindSolution(jobs, m, len(m) - 1)
    return m[-1]

print(BestProfit(jobs))


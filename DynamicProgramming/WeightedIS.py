# | start | finish | weight |
jobs = [[1, 2, 50],
        [3, 5, 20],
        [6, 19, 100],
        [2, 100, 200]]

# Weighted Interval Scheduling
def PrintSolution(jobs, m, job):
    lastJob = LatestCompatible(jobs, job)

    if job is 0:
        return
    if jobs[job][2] + m[lastJob] > m[job - 1]:
        print('Job: ', job)
        if lastJob == 0:
            print('Job: ', lastJob)
        PrintSolution(jobs, m, lastJob)
    else:
        PrintSolution(jobs, m, job - 1)

def LatestCompatible(jobs, job):
    for previousJob in range(job -1, -1, -1):
        if jobs[previousJob][1] <= jobs[job][0]:
            return previousJob
    return -1

def BestSchedule(jobs):
    # Sort by finish time
    jobs.sort(key = lambda x:x[1])

    m = []
    m.append(jobs[0][2])

    for job in range(1, len(jobs)):
        weight = jobs[job][2]
        lastJob = LatestCompatible(jobs, job)
        if lastJob is not -1:
            weight += m[lastJob]
        m.append(max(weight, m[-1]))
    PrintSolution(jobs, m, len(m) - 1)
    return m[-1]

print(BestSchedule(jobs))


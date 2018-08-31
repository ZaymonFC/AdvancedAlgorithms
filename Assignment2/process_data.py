from collections import defaultdict
from matplotlib import pyplot as plt
plt.style.use('fast')
import numpy as np

filename = 'ce final scores new.txt'
data = defaultdict(list)

with open(filename, 'r') as fp:
    for line in fp.readlines():
        values = line.strip().split(',')
        if values[1] == -1337:
            continue
        data[str(values[0])].append(float(values[1]))





with open(filename[:-4] + ' output', 'w') as fp:
    for k, v in data.items():
        fp.write(str(k) + '\t' + str(min(v)))
        fp.write('\n')


# plt.xlabel('Population Size')
# plt.ylabel('Best Score (Average)')


# plt.plot(x, y, 'bo')
# # plt.xticks(rotation='vertical')
# plt.show()
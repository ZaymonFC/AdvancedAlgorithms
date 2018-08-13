from matplotlib import pyplot as plt

x = []
y = []
with open("Molecule.txt") as fp:
    lines = fp.readlines()
    for line in lines:
        coords = line.strip().split(',')
        x.append(coords[0])
        y.append(coords[1])
    fp.close()


plt.plot(x, y, 'o--')
plt.ylim(-10, 10)
plt.xlim(-10, 10)
plt.show()
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Load CSV data
data = pd.read_csv('mandelbrot.csv')

# ranks,exec_time,stddev
# 8,0.71625530000000004,0.021670988081072821
# 16,0.38489480000000004,0.0035466599724247642
# 32,0.20096549999999999,0.0011443878931551106
# 64,0.11387229999999998,0.001854232404527549

def speedup(p):
    return data['exec_time'][0] / data['exec_time'][data['ranks'].searchsorted(p)]

def efficiency(p):
    return speedup(p) / p

sups = [speedup(p) for p in data['ranks']]
effs = [efficiency(p) for p in data['ranks']]

# Plot speedup
plt.plot(data['ranks'], sups, 'o:r', label='Speedup [T(1)/T(p)]')

# Plot efficiency
plt.plot(data['ranks'], effs, 'o--b', label='Efficiency [Speedup/p]')

plt.xscale('log', base=2)

plt.xlabel('Ranks')
plt.ylabel('Speedup/Efficiency')
plt.legend()

plt.title('Mandelbrot strong scalability')

for i in range(len(data)):
    plt.annotate(f'{sups[i]:.2f}', (data['ranks'][i], sups[i]), textcoords="offset points", xytext=(0,10), ha='center')
    plt.annotate(f'{effs[i]:.2f}', (data['ranks'][i], effs[i]), textcoords="offset points", xytext=(0,10), ha='center')

plt.show()

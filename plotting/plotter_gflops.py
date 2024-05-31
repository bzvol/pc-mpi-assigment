import pandas as pd
import matplotlib.pyplot as plt

# Load CSV data
data = pd.read_csv('mandelbrot.csv')

# Plotting
plt.errorbar(data['ranks'], data['gflops'], yerr=data['stddev'], fmt='s:r', label='Gflops/s', capsize=10)

# Set x-axis ticks to powers of 2
plt.xscale('log', base=2)

# Set plot labels and legend
plt.xlabel('Ranks')
plt.ylabel('Gflops/s')
plt.legend()

# Add value labels
for i, txt in enumerate(data['gflops']):
    plt.annotate(f'{txt:.2f}', (data['ranks'][i], data['gflops'][i]), textcoords="offset points", xytext=(0,10), ha='center')

# Set plot title
plt.title('Mandelbrot benchmark')

# Show the graph
plt.show()

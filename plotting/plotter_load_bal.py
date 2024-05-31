import pandas as pd
import matplotlib.pyplot as plt

# Load CSV data
data = pd.read_csv('mandelbrot.csv')

# ranks,load_bal,stddev

# Plotting
plt.plot(data['ranks'], data['load_bal'], 's:g', label='Load balance')

# Set x-axis ticks to powers of 2
plt.xscale('log', base=2)
plt.yticks([0.8, 0.9, 1.0, 1.1, 1.2])

# Set plot labels and legend
plt.xlabel('Ranks')
plt.ylabel('Load balance')
plt.legend()

# Add value labels
for i, txt in enumerate(data['load_bal']):
    plt.annotate(f'{txt:.2f}', (data['ranks'][i], data['load_bal'][i]), textcoords="offset points", xytext=(0,10), ha='center')
    
# Set plot title
plt.title('Mandelbrot load balance')

plt.show()

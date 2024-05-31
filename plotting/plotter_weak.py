import pandas as pd
import matplotlib.pyplot as plt

# Load CSV data
data = pd.read_csv('mandelbrot.csv')

# ranks,exec_time,stddev,mandelbrot_res

data['exec_time'] = data['exec_time'] * 1000

# Plotting
plt.plot(data['ranks'], data['exec_time'], 'o:c', label='Execution time')

plt.xscale('log', base=2)

plt.xlabel('Ranks')
plt.ylabel('Execution time (ms)')

plt.title('Mandelbrot weak scalability')

# Annotate with mandelbrot resolution
for i, txt in enumerate(data['mandelbrot_res']):
    exec_time = round(data['exec_time'][i])
    plt.annotate(f'{exec_time}ms, {txt}', (data['ranks'][i], data['exec_time'][i]), textcoords="offset points", xytext=(0,10), ha='center')

plt.show()

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Load CSV data
data = pd.read_csv('mandelbrot.csv')

# ranks,exec_time,stddev,mandelbrot_res
# 1,0.075897099999999995,0.012367911630101506,"500x500"
# 2,0.015898600000000002,5.8855755878249793e-06,"1000x1000"
# 4,0.0639705,0.0018790072511834593,"2000x2000"
# 8,0.2526756,0.0039103767900293121,"4000x4000"
# 16,0.61072080000000006,0.025677856350560135,"8000x8000"
# 32,1.2929614999999999,0.033151100106180435,"16000x16000"
# 64,2.6226116999999998,0.034363545335282138,"32000x32000"

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

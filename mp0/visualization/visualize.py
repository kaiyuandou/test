import sys
import numpy as np
import matplotlib.pyplot as plt

START_TIME = 10
SIM_TIME = 100

if len(sys.argv) != 2:
    print('Usage: python ' + sys.argv[0] + ' [log_file]')

bandwidth = {}
delay = {}

log_file = open(sys.argv[1], 'r')
for entry in log_file.readlines():
    split_entry = entry.split(',')

    if int(split_entry[0]) not in bandwidth:
        bandwidth[int(split_entry[0])] = int(split_entry[2])
    else:
        bandwidth[int(split_entry[0])] += int(split_entry[2])

    if int(split_entry[0]) not in delay:
        delay[int(split_entry[0])] = [abs(float(split_entry[1]))]
    else:
        delay[int(split_entry[0])].append(abs(float(split_entry[1])))

x = np.linspace(START_TIME, START_TIME + SIM_TIME, SIM_TIME + 1)
y_bandwidth = []
for i in x:
    if i in bandwidth:
        y_bandwidth.append(bandwidth[i])
    else:
        y_bandwidth.append(0)

y_delay_min = []
y_delay_max = []
y_delay_med = []
y_delay_nin = []
for i in x:
    if i in delay:
        y_delay_min.append(min(delay[i]))
        y_delay_max.append(max(delay[i]))
        y_delay_med.append(np.median(delay[i]))
        y_delay_nin.append(np.percentile(delay[i], 90))
    else:
        y_delay_min.append(0)
        y_delay_max.append(0)
        y_delay_med.append(0)
        y_delay_nin.append(0)

plt.plot(x, y_bandwidth, label='Bandwidth')
plt.xlabel('Time in seconds')
plt.ylabel('Bandwidth in bits')
plt.title('Bandwidth Used by Logger Over 100 Seconds of Execution')
plt.legend()
plt.show()

plt.plot(x, y_delay_min, label='Minimum delay')
plt.plot(x, y_delay_max, label='Maximum delay')
plt.plot(x, y_delay_med, label='Median delay')
plt.plot(x, y_delay_nin, label='Ninetieth percentile delay')
plt.xlabel('Time in seconds')
plt.ylabel('Delay in seconds')
plt.title('Logger Delay Statistics Over 100 Seconds of Execution')
plt.legend()
plt.show()

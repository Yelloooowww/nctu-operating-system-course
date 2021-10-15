#!/usr/bin/env python3
import csv
from mpl_toolkits.mplot3d import axes3d
import matplotlib.pyplot as plt
import numpy as np
import math

path_list = ['multithread_int.csv', 'multiprocess_int.csv', \
				'multithread_string.csv', 'multiprocess_string.csv', \
				'multithread_struct.csv', 'multiprocess_struct.csv']

for i,path in enumerate(path_list):
	plt.clf()
	
	X,Y,Z = [],[],[] #X:datasize, Y:thread, Z:time
	with open(path, newline='') as csvfile:
		rows = csv.reader(csvfile, delimiter=',')
		headers = next(rows)
		for row in rows:
			X.append(int(row[0]))
			Y.append(int(row[1]))
			Z.append(int(row[2]))

	plot_x, plot_y = [[] for i in range(12)], [[] for i in range(12)]
	for total_points in range(len(X)):
		plot_x[Y[total_points]-1].append(X[total_points])
		plot_y[Y[total_points]-1].append(Z[total_points])

	for thread in range(12): plt.plot(plot_x[thread],plot_y[thread], '.-')

	plt.xscale("log", basex=2)
	plt.grid()

	if i%2==0:
		plt.legend([str(i+1)+" thread" for i in range(12)])
	else:
		plt.legend([str(i+1)+" process" for i in range(12)])
	plt.xlabel("datasize")
	plt.ylabel("time")
	plt.title(path)
	plt.savefig(path[:-4]+".png")
	# plt.show()

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
	X,Y,Z = [],[],[] #X:datasize, Y:thread, Z:time
	with open(path, newline='') as csvfile:
		rows = csv.reader(csvfile, delimiter=',')
		headers = next(rows)
		for row in rows:
			X.append(int(row[0]))
			Y.append(int(row[1]))
			Z.append(int(row[2]))

	plot_x, plot_y = [[] for i in range(12)], [[] for i in range(12)]
	spec_datasize_plot_x = [[] for i in range(int(math.log(100000000,2)))]
	spec_datasize_plot_y = [[] for i in range(int(math.log(100000000,2)))]

	### specify thread/process ###
	for total_points in range(len(X)):
		plot_x[Y[total_points]-1].append(X[total_points])
		plot_y[Y[total_points]-1].append(Z[total_points])

		spec_datasize_plot_x[int(math.log(X[total_points],2))-1].append(Y[total_points])
		spec_datasize_plot_y[int(math.log(X[total_points],2))-1].append(Z[total_points])

	for thread in range(12): plt.plot(plot_x[thread],plot_y[thread], '.-')

	plt.xscale("log", basex=2)
	plt.grid()

	if i%2==0:
		plt.legend([str(i+1)+" thread" for i in range(12)])
	else:
		plt.legend([str(i+1)+" process" for i in range(12)])
	plt.xlabel("datasize")
	plt.ylabel("time")
	plt.title(path[:-4])
	plt.savefig(path[:-4]+".png")
	# plt.show()
	plt.clf()


	### specify thread/process ###
	legend_list = []
	for k in range(int(math.log(100000000,2))):
		if (k>=15) and (k<25):
			legend_list.append("datasize=2^"+str(k))
			plt.plot(spec_datasize_plot_x[k],spec_datasize_plot_y[k], '.-')

	# plt.yscale("log", basex=2)
	plt.grid()
	plt.legend(legend_list)
	# plt.legend(["datasize=2^"+str(k+1) for k in range(int(math.log(100000000,2)),3)])
	if i%2==0:
		plt.xlabel("thread")
	else:
		plt.xlabel("process")

	plt.ylabel("time")
	plt.title("specify_datasize "+path[:-4])
	plt.savefig("specify_datasize "+path[:-4]+".png")
	plt.clf()

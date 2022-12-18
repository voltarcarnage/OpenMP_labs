from pwn import u32
import matplotlib.pyplot as plt
from prettytable.colortable import ColorTable
import struct as st

def read_OpenMP():
    try:
        data = open('experiment.txt','rb')
        num_of_threads = u32(data.read(4))
        threads = [i+1 for i in range(num_of_threads)]
        iterations = u32(data.read(4))
        time = [[] for i in range(num_of_threads)]
        for i in range(iterations * num_of_threads):
            time[i % num_of_threads].append(float(st.unpack('f',data.read(4))[0]))
    finally:
        data.close()
        return time, threads

def read_MPI():
    f = open("data", "rb")
    max_num_threads = 4
    iteration_count = 10
    times = [[] for i in range(max_num_threads)]
    for i in range(max_num_threads * iteration_count):
        times[i % max_num_threads].append(float(st.unpack("d", f.read(8))[0]))
    threads = [i+1 for i in range(max_num_threads)]
    f.close()
    return times, threads


def plots(times_mpi, threads_mpi, times_openmp, threads_openmp):
    times_avg_mpi = [sum(x)/len(x) for x in times_mpi]
    times_avg_openmp = [sum(x)/len(x) for x in times_openmp]
    plt.title('Time compare', fontsize=20)
    plt.plot(threads_mpi, times_avg_mpi, 'r')
    plt.plot(threads_openmp, times_avg_openmp, 'b')
    plt.xlabel("Threads")
    plt.ylabel("Time")
    plt.legend(["MPI", "OpenMP"])
    plt.show()

    s_mpi = [(sum(times_mpi[0])/len(times_mpi[0]))/(sum(x)/len(x)) for x in times_mpi]
    s_openmp = [(sum(times_openmp[0])/len(times_openmp[0]))/(sum(x)/len(x)) for x in times_openmp]
    plt.title('Acceleration compare', fontsize=20)
    plt.plot(threads_mpi, s_mpi, 'r')
    plt.plot(threads_openmp, s_openmp, 'b')
    plt.xlabel("Threads")
    plt.ylabel("Acceleration")
    plt.legend(["MPI", "OpenMP"])
    plt.show()

    e_mpi = [s_mpi[i]/(i + 1) for i in range(len(s_mpi))]
    e_openmp = [s_openmp[i]/(i + 1) for i in range(len(s_openmp))]
    plt.title('Efficiency compare', fontsize=20)
    plt.plot(threads_mpi, e_mpi, 'r')
    plt.plot(threads_openmp, e_openmp, 'b')
    plt.xlabel("Threads")
    plt.ylabel("Efficiency")
    plt.legend(["MPI", "OpenMP"])
    plt.show()

if __name__ == "__main__":
    data1 = read_MPI()
    data2 = read_OpenMP()
    plots(data1[0], data2[1], data2[0], data2[1])


from sys import argv
import matplotlib.pyplot as plt


def read_file(filename):
    tests_number = int(argv[1])
    threads_number = int(argv[2])

    threads = []
    x = 1
    while x <= threads_number:
        threads.append(x)
        x *= 2

    times = []
    file = open(filename, 'r')
    timing = [float(x) for x in file.readline().split()]
    for i in range(len(threads)):
        x = 0
        for j in range(tests_number):
            x += timing[len(threads) * j + i]
        times.append(x / tests_number)
    file.close()

    return threads, times


def draw_plots(data1, data2):
    plt.title("Execution time", fontsize=25)
    plt.xlabel('Threads')
    plt.ylabel('Time')
    plt.grid(True)
    plt.plot(data1[0], data1[1], 'r')
    plt.plot(data2[0], data2[1], 'b')
    plt.legend(['OpenMP', 'MPI'])
    plt.savefig('images/execution_time.png')
    plt.close()

    acceleration1 = [data1[1][0] / x for x in data1[1]]
    acceleration2 = [data2[1][0] / x for x in data2[1]]
    plt.title("Acceleration", fontsize=25)
    plt.xlabel('Threads')
    plt.ylabel('Acceleration')
    plt.grid(True)
    plt.plot(data1[0], acceleration1, 'r')
    plt.plot(data2[0], acceleration2, 'b')
    plt.legend(['OpenMP', 'MPI'])
    plt.savefig('images/acceleration.png')
    plt.close()

    efficiency1 = [acceleration1[x] / data1[0][x] for x in range(len(data1[0]))]
    efficiency2 = [acceleration2[x] / data2[0][x] for x in range(len(data2[0]))]
    plt.title("Efficiency", fontsize=25)
    plt.xlabel('Threads')
    plt.ylabel('Efficiency')
    plt.grid(True)
    plt.plot(data1[0], efficiency1, 'r')
    plt.plot(data2[0], efficiency2, 'b')
    plt.legend(['OpenMP', 'MPI'])
    plt.savefig('images/efficiency.png')
    plt.close()


if __name__ == '__main__':
    openmp = read_file('openmp.txt')
    mpi = read_file('mpi.txt')
    draw_plots(openmp, mpi)

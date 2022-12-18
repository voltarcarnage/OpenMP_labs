import matplotlib.pyplot as plt

def read_file(filename):
    with open(filename) as file:
        threads = [x + 1 for x in range(int(file.readline()))]
        times = [float(x) for x in file.read().split()]
    return threads, times


def draw_plots(threads, times):
    plt.title('Execution time', fontsize=25)
    plt.xlabel('Threads')
    plt.ylabel('Time')
    plt.bar(threads, times)
    plt.savefig('images/histogram.png')
    plt.close()


if __name__ == '__main__':
    data = read_file('timing.txt')
    draw_plots(data[0], data[1])

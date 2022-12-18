from sys import argv
from random import randint

if __name__ == '__main__':
    size = int(argv[1])
    
    file = open('array.txt', 'w')
    for _ in range(size):
        file.write(str(randint(0, size)) + ' ')
    file.write('\n')
    file.close()

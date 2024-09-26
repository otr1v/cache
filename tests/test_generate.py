import random

tests = open('tests/tests.txt', 'w')

tests.write('1000 ')
tests.write('10000\n')
for n in range(10000):
    seq = str(random.randint(0, 1000000)) + ' '
    tests.write(seq)
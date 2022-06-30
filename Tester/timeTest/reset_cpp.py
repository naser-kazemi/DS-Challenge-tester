import os

if __name__ == '__main__':
    bak = open('timeTest/main_bak.cpp')
    code = open('yourCode/main.cpp', 'w')
    code.write(bak.read())
    bak.close()
    code.close()
    os.remove('timeTest/main_bak.cpp')

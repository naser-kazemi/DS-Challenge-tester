import os

if __name__ == '__main__':
    bak = open('timeTest/main_bak.py')
    code = open('yourCode/main.py', 'w')
    code.write(bak.read())
    bak.close()
    code.close()
    os.remove('timeTest/main_bak.py')

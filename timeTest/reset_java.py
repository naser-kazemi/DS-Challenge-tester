import os

if __name__ == '__main__':
    bak = open('timeTest/MainBak.java')
    code = open('yourCode/Main.java', 'w')
    code.write(bak.read())
    bak.close()
    code.close()
    os.remove('timeTest/MainBak.java')

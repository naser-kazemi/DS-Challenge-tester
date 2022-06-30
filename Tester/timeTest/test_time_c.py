import re

if __name__ == '__main__':
    lines = open("timeTest/c_time.txt").read().split("\n")
    file = open('yourCode/main.c')
    old_code = file.read()
    code_text = old_code
    bak = open('timeTest/main_bak.c', 'w')
    bak.write(old_code)
    bak.close()
    file.close()
    code_text = lines[0] + '\n' + code_text
    reg = re.compile(r'[ ]*// \*{14}')
    m = re.findall(reg, code_text)
    code_text = re.sub(r'// \*{14}', lines[1] + '\n' + lines[2], code_text, count=1)
    l = (len(m[0]) - len(m[0].lstrip()))
    code_text = re.sub(r'// \*{14}', lines[3] + '\n' + ' ' * l + lines[4] + '\n' + ' ' * l + lines[5] + '\n',
                       code_text, count=1)
    file = open('yourCode/main.c', 'w')
    file.write(code_text)

import re

if __name__ == '__main__':
    lines = open("timeTest/cpp_time.txt").read().split("\n")
    file = open('yourCode/main.cpp')
    old_code = file.read()
    code_text = old_code
    bak = open('timeTest/main_bak.cpp', 'w')
    bak.write(old_code)
    bak.close()
    file.close()
    code_text = lines[0] + '\n' + code_text
    reg = re.compile(r'[ ]*// \*{14}')
    m = re.findall(reg, code_text)
    code_text = re.sub(r'// \*{14}', lines[1], code_text, count=1)
    code_text = re.sub(r'// \*{14}', lines[2] + '\n' + ' ' * (len(m[0]) - len(m[0].lstrip())) + lines[3] + '\n',
                       code_text, count=1)
    file = open('yourCode/main.cpp', 'w')
    file.write(code_text)

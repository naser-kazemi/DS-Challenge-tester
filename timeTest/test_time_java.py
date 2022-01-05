import re

if __name__ == '__main__':
    lines = open("timeTest/java_time.txt").read().split("\n")
    file = open('yourCode/Main.java')
    old_code = file.read()
    code_text = old_code
    bak = open('timeTest/MainBak.java', 'w')
    bak.write(old_code)
    bak.close()
    file.close()
    reg = re.compile(r'[ ]*// \*{14}')
    m = re.findall(reg, code_text)
    l = (len(m[0]) - len(m[0].lstrip()))
    code_text = re.sub(r'// \*{14}', lines[0], code_text, count=1)
    code_text = re.sub(r'// \*{14}', lines[1] + '\n' + ' ' * l + lines[2] + '\n' + ' ' * l + lines[3] + '\n',
                       code_text, count=1)
    file = open('yourCode/Main.java', 'w')
    file.write(code_text)

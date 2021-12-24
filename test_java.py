import os
import subprocess


def open_file(filename):
    f = open(filename)
    return f.read()


def check_answer_line(to_be_checked, correct_answer, test_num):
    size = len(correct_answer)
    count = 0
    if len(to_be_checked) != len(correct_answer):
        print('test #' + str(test_num) + " failed",
              end='\n---------------------------------------------------------\n'
                  '---------------------------------------------------------\n\n')
        return False
    for word in to_be_checked:
        if word not in correct_answer:
            print('test #' + str(test_num) + " failed",
                  end='\n---------------------------------------------------------\n'
                      '---------------------------------------------------------\n\n')
            return False
        count += 1
    if size != count:
        print('test #' + str(test_num) + " failed",
              end='\n---------------------------------------------------------\n'
                  '---------------------------------------------------------\n\n')
        return False
    return True


def check_answer(to_be_checked_out, correct_answer_out, test_num):
    to_be_checked_lines = to_be_checked_out.strip().split("\n")
    correct_answer_lines = correct_answer_out.strip().split("\n")
    lines = len(to_be_checked_lines)
    if lines != len(correct_answer_lines):
        print('test #' + str(test_num) + " failed! Number of lines is not equal!",
              end='\n---------------------------------------------------------\n'
                  '---------------------------------------------------------\n\n')
        return False
    for i in range(lines):
        to_be_checked = to_be_checked_lines[i].strip().split(' ')
        correct_answer = correct_answer_lines[i].strip().split(' ')
        if correct_answer[0] != to_be_checked[0]:
            print('test #' + str(test_num) + " failed")
            print("your output:\n" + to_be_checked_lines[i], "correct output:\n" + correct_answer_lines[i],
                  sep='\n---------------------------------------------------------\n',
                  end='\n---------------------------------------------------------\n'
                      '---------------------------------------------------------\n\n')
            return False
        to_be_checked = to_be_checked[1:]
        correct_answer = correct_answer[1:]
        if not check_answer_line(to_be_checked, correct_answer, test_num):
            print("your output:\n" + to_be_checked_lines[i], "correct output:\n" + correct_answer_lines[i],
                  sep='\n---------------------------------------------------------\n',
                  end='\n---------------------------------------------------------\n'
                      '---------------------------------------------------------\n\n')
            return False
    return True


def test_code():
    subprocess.run("javac Main.java", shell=True, capture_output=True, text=True)
    for i in range(1, len(list(filter(lambda x: ".txt" in x, os.listdir('./in')))) + 1):
        _input = open("./in/input" + str(i) + ".txt")
        output = subprocess.run('java Main', shell=True, stdin=_input, capture_output=True, text=True)
        to_be_checked = output.stdout
        correct_answer = open_file("./out/output" + str(i) + ".txt")
        if check_answer(to_be_checked, correct_answer, i):
            print('test #' + str(i) + " passed")
            print("---------------------------------------------------------"
                  "\n---------------------------------------------------------\n")
    files = list(filter(lambda x: ".class" in x, os.listdir('.')))
    for file in files:
        os.remove(file)


if __name__ == '__main__':
    test_code()

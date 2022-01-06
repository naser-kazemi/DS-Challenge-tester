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
        if not check_answer_line(to_be_checked, correct_answer, i):
            print("your output:\n" + to_be_checked_lines[i], "correct output:\n" + correct_answer_lines[i],
                  sep='\n---------------------------------------------------------\n',
                  end='\n---------------------------------------------------------\n'
                      '---------------------------------------------------------\n\n')
            return False
    return True


def test_code(get_time=False):
    time_test_loop = 20.0
    if get_time:
        subprocess.run("python3 timeTest/test_time_cpp.py", shell=True)
    subprocess.run("g++ -std=c++11 ./yourCode/main.cpp -o run", shell=True, capture_output=True, text=True)
    for i in range(1, len(list(filter(lambda x: ".txt" in x, os.listdir('./tests/in')))) + 1):
        _input = open("./tests/in/input" + str(i) + ".txt")
        output = subprocess.run('./run', shell=True, stdin=_input, capture_output=True, text=True)
        to_be_checked = output.stdout
        total_time = 0.0
        output_time = ''
        lines = []
        if get_time:
            for j in range(int(time_test_loop)):
                _input = open("./tests/in/input" + str(i) + ".txt")
                output = subprocess.run('./run', shell=True, stdin=_input, capture_output=True, text=True)
                to_be_checked = output.stdout
                lines = to_be_checked.strip().split('\n')
                output_time = lines[len(lines) - 1]
                try:
                    temp = float(output_time.strip())
                    total_time += temp
                except ValueError:
                    pass
        to_be_checked = '\n'.join(lines[:len(lines) - 1])
        correct_answer = open_file("./tests/out/output" + str(i) + ".txt")
        if check_answer(to_be_checked, correct_answer, i):
            print('test #' + str(i) + " passed")
            print("---------------------------------------------------------"
                  "\n---------------------------------------------------------\n")
        if get_time:
            avg = total_time / time_test_loop
            print('Your code average time in ' + str(int(time_test_loop)) + ' runs is: ' + str(avg) + '\n\n')
    if get_time:
        subprocess.run("python3 timeTest/reset_cpp.py", shell=True)
    os.remove("run")


if __name__ == '__main__':
    test_code(True)

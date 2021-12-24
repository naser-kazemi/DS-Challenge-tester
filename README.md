# DS-Challenge-tester

## How to use the Tester
This is Tester for DS Challenge phase 1. The tester consists of python scripts for testing your code in Java, Python and C++ languages.

All you need to do is to copy all of your project source code files and a "driver code in file a with name":

+ Main.java for Java
+ main.cpp for C++
+ main.py for Python

without any packages in tester directory and run the right script.

For project written in C, C++ tester can be slightly modified which is easy. all you need is to change the command for code compilation in in subprocess.run() function.
Even for some C++ compliers which can complie C code without any problem, this is not needed.

If you have problem running test_python.py script, you may change "python3 main.py" command to "pyhton main.cpp" in subprocess.run() function and there will be no issue.

## Test Cases

Test from #1 to #10 consists of queries with more than one "\S*" in the given pattern and it is not necessary to pass them. But your code has to pass all the other tests. 


This tester will be updated with more features for phase 2 and more acurate diff-checker functions.

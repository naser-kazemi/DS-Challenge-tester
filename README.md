# DS-Challenge-tester

## How to use the Tester
This is a Tester for Data Structures and Algorithms Challenge phase 1. The tester consists of python scripts for testing your code in Java, Python and C++ languages.

All you need to do is to copy all of your project source code files and a "driver code in a file with name":

+ Main.java for Java
+ main.cpp for C++
+ main.py for Python

without any packages in the tester directory and run the right script.

For projects written in C, _test_Cpp.py_ can be slightly modified, which is easy. all you need to do is to change the command for code compilation in the _subprocess.run()_ function.
Although for some C++ compliers which can complie C codes without any problems, this is not needed.

_If you have problem running "test_python.py" script, you may change "python3 main.py" command to "python main.cpp" in the subprocess.run() function and there will be no issues._

## Test Cases

Tests from #1 to #10 consist of queries with more than one "\S*" in the given pattern and it is not necessary to pass them. But your code has to pass all the other tests. 


**This tester will be updated with more features for phase 2 and more acurate diff-checker functions.**

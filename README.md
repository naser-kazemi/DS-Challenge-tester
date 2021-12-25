# DS-Challenge-tester

## How to use the Tester
This is a Tester for Data Structures and Algorithms Challenge phase 1. The tester consists of python scripts for testing your code in Java, Python and C++ languages.

All you need to do is to copy all of your project source code files and a "driver code in a file with name":

+ Main.java for Java
+ main.cpp for C++
+ main.py for Python

without any packages in the "yourCode" directory and run the right script.

_For projects written in C, _test_Cpp.py_ can be slightly modified, which is easy. all you need to do is to change the command for code compilation in the _subprocess.run()_ function.
Although for some C++ compliers which can complie C codes without any problems, this is not needed._

## Common Issues

+ If you have problem running "test_python.py" script(for example you fail all tests but when run your own code and get the correct output), you may change "python3 ./yourCode/main.py" command to "python ./yourCode/main.py" in the subprocess.run() function and there will be no issues.

+ If you have problem running "test_Cpp.py script"(in same cases as above), you may have problem with your code compilation. For example if you are using "bits/stdc++.h" header file, your system compiler(which the script uses to compile the code with) may not have all the libraries included in "bits/stdc++.h" and thus you are not able to compile your code, and you get unexpected results from the tester. In this case all you neeed to do is to edit the "stdc++.h" so that you can complie the code.

## Test Cases

Tests from #1 to #10 consist of queries with more than one "\S*" in the given pattern and it is not necessary to pass them. But your code has to pass all the other tests. 


**This tester will be updated with more features for phase 2 and more accurate diff-checker functions.**
# DS-Challenge-tester

## How to use the Tester
This is a Tester for Data Structures and Algorithms Challenge phase 2 based on the previous version of tester designed for phase 1. The tester consists of python scripts for testing your code in Java, Python and C/C++ languages. This updated version of tester is able to evaluate the time interval of you code and show you the result.

## Test for Correctness

All you need to do is to copy all of your project source code files and a "driver code in a file with name":

+ Main.java for Java
+ main.cpp for C++
+ main.c for C
+ main.py for Python

without any packages in the `yourCode` directory and run the right script.

_For projects written in C, also _test_cpp.py_ can be slightly modified, which is easy. all you need to do is to change the command for code compilation in the _subprocess.run()_ function.
Although for some C++ compliers which can complie C codes without any problems, this is not needed._


## Test for Time and Efficiency

To use the time testing feature of the tester, all you need to do is to do the following:

1. copy lines `# **************` for Python language and `// **************` for Java and C/C++ as indicators for the tester, in places you want to evaluate the time of execution between.
2. pass the `True` argumant to get `test_code()` function in the corresponding script of the language your code is in.
3. run the tester script.

##### For example

if you've written your code in C++, you would modify your `main` function as following:

```C++
int main() {
    // code to make your model and read inputs
    
    // **************
    
    // code to find the result of queries
    
    // **************
}
```
and pass the `True` argumant to get `test_code()` in `test_cpp.py` as following:

```python
if __name__ == '__main__':
    test_code(True)
```
The tester replaces `// **************` lines with the lines of code recommended for time evaluation in [here](https://quera.ir/course/assignments/37693/problems/129344) and runs the test with the execution time for each test shown in output. After running all test the indicator lines will be put back at there place and you can test your code again easily.

## Common Issues

+ If you have problem running `test_python.py` script(for example you fail all tests but when run your own code and get the correct output), you may change `python3 ./yourCode/main.py` command to `python ./yourCode/main.py` in the subprocess.run() function and there will be no issues.

+ For Java you have to add a line `package yourCode;` at the beginning of each java source code file.

+ If you have problem running `test_cpp.py script`(in same cases as above), you may have problem with your code compilation. For example if you are using `bits/stdc++.h` header file, your system compiler(which the script uses to compile the code with) may not have all the libraries included in `bits/stdc++.h` and thus you are not able to compile your code, and you get unexpected results from the tester. In this case all you neeed to do is to edit the `stdc++.h` so that you can complie the code.

## Test Cases

Tests from #1 to #10 consist of queries with more than one `\S*` in the given pattern and it is not necessary to pass them. But your code has to pass all the other tests.

### Next Updates
In the next updates we will have other features for time testing such as "average execution time", more accurate "diff-checker functions" and also more test cases.


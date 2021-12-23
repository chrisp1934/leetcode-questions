import subprocess
import sys
import os

def print_help_message():
    print("Usage: python3 two_sum.py [OPTION] [DIRECTORY] | [INPUT FILE 1] [INPUT FILE 2] ...")
    print("Runs a set of test cases on a desired implementation of the twoSum function.\n")
    print("-d\tprogram will run all test cases in the directory specified.")
    print("-f\tprogram will run all test cases in files specified.")

def test_directory():
    output, error = run_bash_command("ls " + sys.argv[2])
    test_case_list = get_test_case_list(output)

    total_time, any_test_cases_failed, no_errors = run_test_cases(test_case_list)
    print_test_output(test_case_list, total_time, any_test_cases_failed, no_errors)

def test_files():
    test_case_list = sys.argv[2:len(sys.argv)]
    total_time, any_test_cases_failed, no_errors = run_test_cases(test_case_list)
    print_test_output(test_case_list, total_time, any_test_cases_failed, no_errors)

def run_bash_command(bash_command: str):
    process = subprocess.Popen(bash_command.split(), stdout = subprocess.PIPE)
    output, error = process.communicate()
    return output, error

def get_test_case_list(string_of_files: str):
    output_string = str(string_of_files)[2:-1]
    test_case_list = output_string.split("\\n")
    test_case_list = test_case_list[0:len(test_case_list) - 1]
    for i, test_case in enumerate(test_case_list):
        test_case_list[i] = sys.argv[2] + "/" + test_case
    return test_case_list

def run_test_cases(test_case_list: list):
    total_time = 0
    any_test_cases_failed = False
    no_errors = True
    
    for test_case in test_case_list:
        try:
            output, error = run_bash_command("./two_sum_c -f " + test_case)
            test_case_time = float(output)
        except FileNotFoundError:
            sys.stderr.write("ERROR: Executable not found.\n")
            no_errors = False
            break
        except ValueError:
            sys.stderr.write("ERROR: Invalid output from executable.\n")
            no_errors = False
            break
        except:
            sys.stderr.write("ERROR: Something bad happened.\n")
            no_errors = False
            break
        else:
            space_size = os.get_terminal_size().columns - len("[PASSED]") - len(test_case) - 1
            if (test_case_time >= 0):
                total_time += test_case_time
                print(test_case + ":" + " " * space_size + green() + "[PASSED]" + reset_color())
            else:
                print(test_case + ":" + " " * space_size + red() + "[FAILED]" + reset_color())
                any_test_cases_failed = True
        
    return total_time, any_test_cases_failed, no_errors

def print_test_output(test_case_list: list, total_time: float, any_test_cases_failed: bool, no_errors: bool):
    if (not any_test_cases_failed and no_errors):
        print ("All test cases passed.")
    if (no_errors):
        print("Total time for " + str(len(test_case_list)) + " test cases: " + str(total_time) + " seconds.")

def reset_color():
    return "\033[0m"

def red():
    return "\033[0;31m"

def green():
    return "\033[0;32m"

if (len(sys.argv) == 2 and sys.argv[1] == "-h"):
    print_help_message()
elif (len(sys.argv) == 3 and sys.argv[1] == "-d"):
    test_directory()
elif (len(sys.argv) >= 3 and sys.argv[1] == "-f"):
    test_files()
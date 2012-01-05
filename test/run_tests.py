import os
import commands

clang_loc = "~/Clang/bin/clang"
llvm_loc = "../build/Release+Asserts/bin/llc"

bash_warning = "\033[1;33m"
bash_change = "\033[1;35m"
bash_success = "\033[1;32m"
bash_normal = "\033[1;m"

def success(text):
  print bash_success + text + bash_normal

def warn(text):
  print bash_warning + text + bash_normal

def changed(text):
  print bash_change + text + bash_normal

def log_error(text):
  # TODO: Make this actually log.
  print "\033[0;31m" + text + "\033[1;m"

def run_clang(input_file, output_file):
  command = clang_loc + " -emit-llvm " + input_file + " -c -o " + output_file
  status, output = commands.getstatusoutput(command)
  return status, output

def run_llc(input_file, output_file):
  command = llvm_loc + " -march=arcompact -filetype=asm " + input_file + \
      " -o " + output_file
  status, output = commands.getstatusoutput(command)
  return status, output

def main():
  for entry in os.listdir("."):
    # Skip top-level files (including this one!)
    if not os.path.isdir(entry):
      continue

    test_folder = "./" + entry
    test_c = test_folder + "/test.c"
    test_bc = test_folder + "/test.bc"
    test_s = test_folder + "/test.s"
    old_s = test_folder + "/old.s"

    c_file_exists = os.path.isfile(test_c)
    bc_file_exists = os.path.isfile(test_bc)

    if bc_file_exists and c_file_exists:
      if os.path.getmtime(test_c) > os.path.getmtime(test_bc):
        status, output = run_clang(test_c, test_bc)
        if status != 0:
          warn("WARNING! " + entry + ":\tUnable to compile test.c. " + \
              "Error message written to test_log.txt")
          log_error(output)
          continue
      status, output = run_llc(test_bc, test_s)
      if status != 0:
        warn("WARNING! " + entry + ":\tUnable to compile test.bc. " + \
            "Error message written to test_log.txt")
        log_error(output)
        continue
    elif c_file_exists:
      status, output = run_clang(test_c, test_bc)
      if status != 0:
        warn("WARNING! " + entry + ":\tUnable to compile test.c. " + \
            "Error message written to test_log.txt")
        log_error(output)
        continue
      status, output = run_llc(test_bc, test_s)
      if status != 0:
        warn("WARNING! " + entry + ":\tUnable to compile test.bc. " + \
            "Error message written to test_log.txt")
        log_error(output)
        continue
    else:
      warn("WARNING! " + entry + ":\tUnable to find test.c or test.bc files.")
      continue

    if os.path.isfile(old_s):
      command = "diff " + test_s + " " + old_s
      status, output = commands.getstatusoutput(command)
      if status == 0:
        success(entry + ":\tPassed.")
      else:
        changed(entry + ":\tDetected difference between test.s and old.s.")
    else:
      changed(entry + ":\tNew test detected. Please examine the test.s output, and move it to 'old.s'.")

if __name__ == "__main__":
  main()

#!/usr/bin/env python

import os
import commands

clang_loc = "~/Clang/bin/clang"
llvm_loc = "../build/Release+Asserts/bin/llc"
log_file = None

# Captures the necessary output formats to make bash show colour.
class OutputType:
  SUCCESS = "\033[1;32m"  # Green.
  INFO = "\033[1;35m"     # Magenta.
  WARNING = "\033[1;33m"  # Yellow.
  ERROR = "\033[0;31m"    # Red.

# Prints a type of text (usually given as a member of OutputType)
# to stdout.
def output_text(type, text):
  # \033[1;m returns bash to it's normal colour.
  print type + text + "\033[1;m"

# Logs an error to the log_file.
def log_error(test, text):
  if log_file is not None:
    log_file.write(test)
    log_file.write(":\n")
    log_file.write(text)
    log_file.write("\n\n")

# Runs Clang on the given input filename, producing a file with the
# given output filename.
def run_clang(input_file, output_file):
  command = clang_loc + " -emit-llvm " + input_file + " -c -o " + output_file
  status, output = commands.getstatusoutput(command)
  return status, output

# Runs llc on the given input filename, producing a file with the
# given output filename.
def run_llc(input_file, output_file):
  command = llvm_loc + " -march=arcompact -filetype=asm " + input_file + \
      " -o " + output_file
  status, output = commands.getstatusoutput(command)
  return status, output


def main():
  # First clear the log file.
  global log_file
  try:
    log_file = open("test_log.txt", "w")
  except IOError:
    output_text(OutputType.WARNING, "WARNING! Unable to open log file. " + \
        "Subsequent attempts to log will not work!\n")

  # Run each test.
  for entry in sorted(os.listdir(".")):
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
          output_text(OutputType.WARNING, entry + ":\tWARNING! " + \
              "Unable to compile test.c. " + \
              "Error message written to test_log.txt")
          log_error(entry, output)
          continue
      status, output = run_llc(test_bc, test_s)
      if status != 0:
        output_text(OutputType.WARNING, entry + ":\tWARNING! " + \
            "Unable to compile test.bc. Error message written to test_log.txt")
        log_error(entry, output)
        continue
    elif c_file_exists:
      status, output = run_clang(test_c, test_bc)
      if status != 0:
        output_text(OutputType.WARNING, entry + ":\tWARNING! " + \
            "Unable to compile test.c. Error message written to test_log.txt")
        log_error(entry, output)
        continue
      status, output = run_llc(test_bc, test_s)
      if status != 0:
        output_text(OutputType.WARNING, entry + ":\tWARNING! " + \
            "Unable to compile test.bc. Error message written to test_log.txt")
        log_error(entry, output)
        continue
    else:
      output_text(OutputType.WARNING, entry + ":\tWARNING! " + \
          "Unable to find a test.c or test.bc file.")
      continue

    if os.path.isfile(old_s):
      command = "diff " + test_s + " " + old_s
      status, output = commands.getstatusoutput(command)
      if status == 0:
        pass
        output_text(OutputType.SUCCESS, entry + ":\tPassed.")
      else:
        output_text(OutputType.INFO, entry + \
            ":\tDetected difference between test.s and old.s.")
    else:
      output_text(OutputType.INFO, entry + ":\tNew test detected. " + \
          "Please examine the test.s output, and move it to 'old.s'.")

if __name__ == "__main__":
  main()

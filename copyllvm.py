#!/usr/bin/env python

import commands
import os.path
import sys

files_to_save = ["llvm/configure", "llvm/include/llvm/ADT/Triple.h",
                 "llvm/lib/Support/Triple.cpp"]

def main(llvm_location):
  if not os.path.isdir(llvm_location):
    print "Unable to find given directory: '" + llvm_location + "'"
    return

  # Check that there are no staged/unadded files (for tidyness.)
  status, output = commands.getstatusoutput("git status")
  if status != 0:
    print "ERROR: git-status returned with an error."
    print
    print output 
    return

  if output.find("On branch master\nnothing to commit") < 0:
    print "ERROR: There are un-committed changes on your branch."
    print
    print output
    return

  # Backup the saved files.
  for file in files_to_save:
    if not os.path.isfile(file):
      print "WARNING: Unable to save file: '" + file + "'"
      continue

    command = "cp " + file + " " + file + ".bak"
    status, output = commands.getstatusoutput(command)

    if status != 0:
      print "WARNING: Unable to save file: '" + file + "'"

  # Now pull in llvm. Luckily we can use git to backup if things go really wrong. lol
  command = "cp -rn " + llvm_location + "/* llvm"
  status, output = commands.getstatusoutput(command)
  if status != 0:
    print "ERROR: Unable to copy in the LLVM location."
    print
    print output
    return

  # Move back the saved files.
  for file in files_to_save:
    command = "mv " + file + ".bak " + file
    status, output = commands.getstatusoutput(command)
    if status != 0:
      print "WARNING: Unable to restore saved file: '" + file + "'"

  # Finally, check with git that nothing unexpected has changed.
  status, output = commands.getstatusoutput("git status")
  if status != 0:
    print "ERROR: git-status returned with an error."
    print
    print output 
    return

  if output.find("On branch master\nnothing to commit") < 0:
    print "WARNING: Git has detected changes. Something may have gone wrong;" + \
          "consider running 'git revert --hard HEAD'!"
    print
    print output
  else:
    print ""
    print "LLVM copied successfully."

if __name__ == "__main__":
  if len(sys.argv) == 2:
    main(sys.argv[1])
  else:
    print "Usage: python copyllvm.py llvm_location"

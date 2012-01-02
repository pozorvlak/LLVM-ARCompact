#!/usr/bin/env python

import commands
import os.path
import sys

files_to_save = ["llvm/configure", "llvm/include/llvm/ADT/Triple.h",
                 "llvm/lib/Support/Triple.cpp"]

def main(llvm_location):
  if not os.path.isdir(llvm_location):
    print "LLVMERROR"
    return

  status, output = commands.getstatusoutput("git status")
  if status != 0:
    print "ERROR"
    return

  if output.find("On branch master\nnothing to commit") < 0:
    print "ERROR2"
    return

  for file in files_to_save:
    if not os.path.isfile(file):
      print "WARNING"
      continue

    command = "cp " + file + " " + file + ".bak"
    status, output = commands.getstatusoutput(command)

    if status != 0:
      print "WARNING"

  # Now pull in llvm. Luckily we can use git to backup if things go really wrong. lol
  command = "cp -rf " + llvm_location + "llvm"
  print command

if __name__ == "__main__":
  if len(sys.argv) == 2:
    main(sys.argv[1])
  else:
    print "ERROR3"

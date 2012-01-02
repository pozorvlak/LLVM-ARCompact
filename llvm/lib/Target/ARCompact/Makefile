##===-------------- lib/Target/ARCompact/Makefile -------------------------===##
#
#                     The LLVM Compiler Infrastructure
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
#
##===----------------------------------------------------------------------===##

LEVEL = ../../..
LIBRARYNAME = LLVMARCompactCodeGen
TARGET = ARCompact

# Make sure that tblgen is run, first thing.
BUILT_SOURCES = ARCompactGenRegisterInfo.inc ARCompactGenInstrInfo.inc \
		ARCompactGenAsmWriter.inc ARCompactGenDAGISel.inc \
		ARCompactGenSubtargetInfo.inc ARCompactGenCallingConv.inc

DIRS = TargetInfo MCTargetDesc

include $(LEVEL)/Makefile.common

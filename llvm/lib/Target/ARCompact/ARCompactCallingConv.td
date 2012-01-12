//===------ ARCompactCallingConv.td - Calling Conventions ARCompact -------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This describes the calling conventions for the ARCompact architectures.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Return Value Calling Conventions
//===----------------------------------------------------------------------===//

// 32-bit C return-value convention.
def RetCC_ARCompact32 : CallingConv<[
  // TODO: Check if R0 through R7 should be used for return values.
  CCIfType<[i32], CCAssignToReg<[R0, R1, R2, R3, R4, R5, R6, R7]>>
]>;

// 32-bit C Calling convention.
def CC_ARCompact32 : CallingConv<[
  // The first 8 int-32 arguments of non var-arg functions get passed in
  // in registers R0-R7.
  CCIfNotVarArg<CCIfType<[i32], CCAssignToReg<[R0, R1, R2, R3, R4, R5, R6, R7]>>>,

  // Everything else is assigned to the stack in 4-byte aligned units.
  CCIfType<[i32], CCAssignToStack<4, 4>>
]>;


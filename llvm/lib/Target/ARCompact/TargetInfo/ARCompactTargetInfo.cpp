//===------ ARCompactTargetInfo.cpp - ARCompact Target Implementation -----===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "ARCompact.h"
#include "llvm/Module.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

Target llvm::TheARCompactEncoreTarget;

extern "C" void LLVMInitializeARCompactTargetInfo() {
  RegisterTarget<Triple::arcompact> X(TheARCompactEncoreTarget, "arcompact",
      "ARCompact");
}

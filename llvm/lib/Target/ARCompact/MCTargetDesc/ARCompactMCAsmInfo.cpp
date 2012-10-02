//===-------- ARCompactMCAsmInfo.cpp - ARCompact asm properties -----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declarations of the ARCompactMCAsmInfo properties.
//
//===----------------------------------------------------------------------===//

#include "ARCompactMCAsmInfo.h"
#include "llvm/ADT/Triple.h"

using namespace llvm;

ARCompactELFMCAsmInfo::ARCompactELFMCAsmInfo(const Target &T, StringRef TT) {
  UsesELFSectionDirectiveForBSS = true;
  IsLittleEndian = true;
  Triple TheTriple(TT);

  CommentString = ";";
  SupportsDebugInformation = true;
}

//=====---- ARCompactSubtarget.h - Define Subtarget for the ARCOMPACT ----====//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the ARCOMPACT specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#ifndef ARCOMPACT_SUBTARGET_H
#define ARCOMPACT_SUBTARGET_H

#include "llvm/Target/TargetSubtargetInfo.h"
#include <string>

#define GET_SUBTARGETINFO_HEADER
#include "ARCompactGenSubtargetInfo.inc"

namespace llvm {
class StringRef;

class ARCompactSubtarget : public ARCompactGenSubtargetInfo {
public:
  ARCompactSubtarget(const std::string &TT, const std::string &CPU,
                 const std::string &FS);

  /// Parses features string that sets specified subtarget options.
  /// Auto-generated by tablegen.
  void ParseSubtargetFeatures(StringRef CPU, StringRef FS);

  std::string getDataLayout() const {
    const char *p;
    p = "e-p:32:32:32";
    return std::string(p);
  }
};

} // end namespace llvm

#endif

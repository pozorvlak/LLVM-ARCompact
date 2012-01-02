//===------ ARCompactSelectionDAGInfo.h - ARCompact SelectionDAG Info -----===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the ARCompact subclass for TargetSelectionDAGInfo.
//
//===----------------------------------------------------------------------===//

#ifndef ARCOMPACTSELECTIONDAGINFO_H
#define ARCOMPACTSELECTIONDAGINFO_H

#include "llvm/Target/TargetSelectionDAGInfo.h"

namespace llvm {

class ARCompactTargetMachine;

class ARCompactSelectionDAGInfo : public TargetSelectionDAGInfo {
public:
  explicit ARCompactSelectionDAGInfo(const ARCompactTargetMachine &TM);
  ~ARCompactSelectionDAGInfo();
};

}

#endif

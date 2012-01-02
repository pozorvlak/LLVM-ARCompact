//===----- ARCompactSelectionDAGInfo.cpp - ARCompact SelectionDAG Info ----===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the ARCompactSelectionDAGInfo class.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "arcompact-selectiondag-info"
#include "ARCompactTargetMachine.h"
using namespace llvm;

ARCompactSelectionDAGInfo::ARCompactSelectionDAGInfo(
    const ARCompactTargetMachine &TM) : TargetSelectionDAGInfo(TM) {
}

ARCompactSelectionDAGInfo::~ARCompactSelectionDAGInfo() {
}


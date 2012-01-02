//===-- ARCompactISelLowering.cpp - ARCompact DAG Lowering Implementation -===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the interfaces that ARCompact uses to lower LLVM code 
// into a selection DAG.
//
//===----------------------------------------------------------------------===//

#include "ARCompactISelLowering.h"
#include "ARCompactTargetMachine.h"
#include "llvm/DerivedTypes.h"
#include "llvm/Function.h"
#include "llvm/Module.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/ADT/VectorExtras.h"
#include "llvm/Support/ErrorHandling.h"
using namespace llvm;

#include "ARCompactGenCallingConv.inc"

ARCompactTargetLowering::ARCompactTargetLowering(TargetMachine &TM)
    : TargetLowering(TM, new TargetLoweringObjectFileELF()) {
  // Set up the register classes.
  addRegisterClass(MVT::i32, ARC::CPURegsRegisterClass);

  // Compute the derived properties from the register classes.
  computeRegisterProperties();
}

SDValue ARCompactTargetLowering::LowerOperation(SDValue Op, SelectionDAG &DAG) 
    const {
  switch (Op.getOpcode()) {
    default:
      assert(0 && "Unimplemented operand!");
      return SDValue();
  }
}

/// This hook must be implemented to lower the incoming (formal) arguments,
/// described by the Ins array, into the specified DAG. The implementation
/// should fill in the InVals array with legal-type argument values, and return
/// the resulting token chain value.
SDValue ARCompactTargetLowering::LowerFormalArguments(SDValue Chain, 
    CallingConv::ID CallConv, bool isVarArg, 
    const SmallVectorImpl<ISD::InputArg> &Ins, DebugLoc dl, SelectionDAG &DAG,
    SmallVectorImpl<SDValue> &InVals) const {
  MachineFunction &MF = DAG.getMachineFunction();
  MachineFrameInfo *MFI = MF.getFrameInfo();

  // Assign locations to all of the incoming arguments.
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, isVarArg, DAG.getMachineFunction(),
     getTargetMachine(), ArgLocs, *DAG.getContext());
  CCInfo.AnalyzeFormalArguments(Ins, CC_ARCompact32);

  // TODO: Handle varargs.
  assert(!isVarArg && "Varargs not supported yet");


  // Push the arguments onto the InVals vector.
  SDValue ArgValue;
  for (unsigned int i = 0, e = ArgLocs.size(); i != e; ++i) {
    CCValAssign &VA = ArgLocs[i];
    if (VA.isRegLoc()) {
      // Arguments passed in registers.

      TargetRegisterClass *RC = ARC::CPURegsRegisterClass;
      unsigned int Register = MF.addLiveIn(VA.getLocReg(), RC);
      EVT RegisterValueType = VA.getLocVT();
      ArgValue = DAG.getCopyFromReg(Chain, dl, Register, RegisterValueType);

      InVals.push_back(ArgValue);
    } else {
      // Sanity check
      assert(VA.isMemLoc());

      // Load the argument to a virtual register
      unsigned ObjSize = VA.getLocVT().getSizeInBits()/8;

      if (ObjSize != 4) {
        llvm_unreachable("Memory argument is wrong size - not 32 bit!");
      }

      // Create the frame index object for this incoming parameter...
      int FI = MFI->CreateFixedObject(ObjSize, VA.getLocMemOffset(), true);

      // Create the SelectionDAG nodes corresponding to a load from this 
      // parameter.
      SDValue FIN = DAG.getFrameIndex(FI, MVT::i32);
      InVals.push_back(DAG.getLoad(VA.getLocVT(), dl, Chain, FIN, 
                                   MachinePointerInfo::getFixedStack(FI),
                                   false, false, false, 0));
    }
  }

  return Chain;
}

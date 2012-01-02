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

const char* ARCompactTargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch (Opcode) {
    case ARCISD::RET_FLAG:   return "ARCISD::RET_FLAG";
    default:                 return 0;
  }
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

SDValue ARCompactTargetLowering::LowerReturn(SDValue Chain, 
    CallingConv::ID CallConv, bool isVarArg, 
    const SmallVectorImpl<ISD::OutputArg> &Outs, 
    const SmallVectorImpl<SDValue> &OutVals, DebugLoc dl, SelectionDAG &DAG) 
    const {
  MachineFunction &MF = DAG.getMachineFunction();

  // CCValAssign - represent the assignment of the return value to locations.
  SmallVector<CCValAssign, 16> RVLocs;

  // CCState - Info about the registers and stack slot.
  CCState CCInfo(CallConv, isVarArg, DAG.getMachineFunction(),
     DAG.getTarget(), RVLocs, *DAG.getContext());

  // Analize return values.
  CCInfo.AnalyzeReturn(Outs, RetCC_ARCompact32);

  // If this is the first return lowered for this function, add the regs to the
  // liveout set for the function.
  if (MF.getRegInfo().liveout_empty()) {
    for (unsigned i = 0; i != RVLocs.size(); ++i)
      if (RVLocs[i].isRegLoc()) { 
        MF.getRegInfo().addLiveOut(RVLocs[i].getLocReg());
      }
  }

  SDValue Flag;

  // Copy the result values into the output registers.
  for (unsigned i = 0; i != RVLocs.size(); ++i) {
    CCValAssign &VA = RVLocs[i];
    assert(VA.isRegLoc() && "Can only return in registers!");

    Chain = DAG.getCopyToReg(Chain, dl, VA.getLocReg(), OutVals[i], Flag);

    // Guarantee that all emitted copies are stuck together with flags.
    Flag = Chain.getValue(1);
  }

  // TODO: Check this value.
  unsigned int RetAddrOffset = 8; // Call Instruction + Delay Slot
  SDValue RetAddrOffsetNode = DAG.getConstant(RetAddrOffset, MVT::i32);

  if (Flag.getNode()) {
    return DAG.getNode(ARCISD::RET_FLAG, dl, MVT::Other, Chain,
        RetAddrOffsetNode, Flag);
  }

  return DAG.getNode(ARCISD::RET_FLAG, dl, MVT::Other, Chain,
      RetAddrOffsetNode);
}

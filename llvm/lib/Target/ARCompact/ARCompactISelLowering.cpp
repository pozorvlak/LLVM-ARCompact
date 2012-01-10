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

  // Do not have division, so int-division is expensive.
  setIntDivIsCheap(false);

  // We don't have 1-bit extension (i.e. for bools).
  setLoadExtAction(ISD::EXTLOAD,  MVT::i1,  Promote);
  setLoadExtAction(ISD::SEXTLOAD, MVT::i1,  Promote);
  setLoadExtAction(ISD::ZEXTLOAD, MVT::i1,  Promote);

  setOperationAction(ISD::GlobalAddress,  MVT::i32,   Custom);
  setOperationAction(ISD::BR_CC,          MVT::i32,   Custom);
  setOperationAction(ISD::BRCOND,         MVT::Other, Expand);
}

const char* ARCompactTargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch (Opcode) {
    case ARCISD::CALL:        return "ARCISD::CALL";
    case ARCISD::CMP:         return "ARCISD::CMP";
    case ARCISD::BR_CC:       return "ARCISD::BR_CC";
    case ARCISD::Wrapper:     return "ARCISD::Wrapper";
    case ARCISD::RET_FLAG:    return "ARCISD::RET_FLAG";
    default:                  return 0;
  }
}

SDValue ARCompactTargetLowering::LowerOperation(SDValue Op, SelectionDAG &DAG)
    const {
  switch (Op.getOpcode()) {
    case ISD::GlobalAddress:    return LowerGlobalAddress(Op, DAG);
    case ISD::BR_CC:            return LowerBR_CC(Op, DAG);
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

SDValue ARCompactTargetLowering::LowerCall(SDValue Chain, SDValue Callee,
    CallingConv::ID CallConv, bool isVarArg, bool &isTailCall,
    const SmallVectorImpl<ISD::OutputArg> &Outs,
    const SmallVectorImpl<SDValue> &OutVals,
    const SmallVectorImpl<ISD::InputArg> &Ins,
    DebugLoc dl, SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {
  // ARCompact does not support tail calls yet.
  isTailCall = false;

  // Analyze operands of the call, assigning locations to each operand.
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, isVarArg, DAG.getMachineFunction(),
      getTargetMachine(), ArgLocs, *DAG.getContext());

  CCInfo.AnalyzeCallOperands(Outs, CC_ARCompact32);

  // Get a count of how many bytes are to be pushed on the stack.
  unsigned NumBytes = CCInfo.getNextStackOffset();

  Chain = DAG.getCALLSEQ_START(Chain,
      DAG.getConstant(NumBytes, getPointerTy(), true));

  SmallVector<std::pair<unsigned, SDValue>, 4> RegsToPass;
  SmallVector<SDValue, 12> MemOpChains;
  SDValue StackPtr;

  // Walk the register/memloc assignments, inserting copies/loads.
  for (unsigned i = 0, e = ArgLocs.size(); i != e; ++i) {
    CCValAssign &VA = ArgLocs[i];

    SDValue Arg = OutVals[i];

    // Promote the value if needed.
    switch (VA.getLocInfo()) {
      case CCValAssign::Full:
        break;
      case CCValAssign::SExt:
        Arg = DAG.getNode(ISD::SIGN_EXTEND, dl, VA.getLocVT(), Arg);
        break;
      case CCValAssign::ZExt:
        Arg = DAG.getNode(ISD::ZERO_EXTEND, dl, VA.getLocVT(), Arg);
        break;
      case CCValAssign::AExt:
        Arg = DAG.getNode(ISD::ANY_EXTEND, dl, VA.getLocVT(), Arg);
        break;
      default:
        llvm_unreachable("Unknown loc info!");
    }

    // Arguments that can be passed in a register must be kept in the
    // RegsToPass vector.
    if (VA.isRegLoc()) {
      RegsToPass.push_back(std::make_pair(VA.getLocReg(), Arg));
    } else {
      // Sanity check.
      assert(VA.isMemLoc());

      // Get the stack pointer if needed.
      if (StackPtr.getNode() == 0) {
        StackPtr = DAG.getCopyFromReg(Chain, dl, ARC::SP, getPointerTy());
      }

      SDValue PtrOff = DAG.getNode(ISD::ADD, dl, getPointerTy(), StackPtr,
          DAG.getIntPtrConstant(VA.getLocMemOffset()));

      MemOpChains.push_back(DAG.getStore(Chain, dl, Arg, PtrOff,
          MachinePointerInfo(),false, false, 0));
    }
  }

  // Transform all store nodes into one single node because all store nodes are
  // independent of each other.
  if (!MemOpChains.empty()) {
    Chain = DAG.getNode(ISD::TokenFactor, dl, MVT::Other, &MemOpChains[0],
        MemOpChains.size());
  }

  // Build a sequence of copy-to-reg nodes chained together with token chain and
  // flag operands which copy the outgoing args into registers.  The InFlag in
  // necessary since all emitted instructions must be stuck together.
  SDValue InFlag;
  for (unsigned i = 0, e = RegsToPass.size(); i != e; ++i) {
    Chain = DAG.getCopyToReg(Chain, dl, RegsToPass[i].first,
        RegsToPass[i].second, InFlag);
    InFlag = Chain.getValue(1);
  }

  // If the callee is a GlobalAddress node (quite common, every direct call is)
  // turn it into a TargetGlobalAddress node so that legalize doesn't hack it.
  // Likewise ExternalSymbol -> TargetExternalSymbol.
  if (GlobalAddressSDNode *G = dyn_cast<GlobalAddressSDNode>(Callee)) {
    Callee = DAG.getTargetGlobalAddress(G->getGlobal(), dl, MVT::i32);
  }
  else if (ExternalSymbolSDNode *E = dyn_cast<ExternalSymbolSDNode>(Callee)) {
    Callee = DAG.getTargetExternalSymbol(E->getSymbol(), MVT::i32);
  }

  // Returns a chain & a flag for retval copy to use.
  SDVTList NodeTys = DAG.getVTList(MVT::Other, MVT::Glue);
  SmallVector<SDValue, 8> Ops;
  Ops.push_back(Chain);
  Ops.push_back(Callee);

  // Add the argument registers to the end of the list so that they are
  // known live into the call.
  for (unsigned i = 0, e = RegsToPass.size(); i != e; ++i) {
    Ops.push_back(DAG.getRegister(RegsToPass[i].first,
        RegsToPass[i].second.getValueType()));
  }

  if (InFlag.getNode()) {
    Ops.push_back(InFlag);
  }

  Chain = DAG.getNode(ARCISD::CALL, dl, NodeTys, &Ops[0], Ops.size());
  InFlag = Chain.getValue(1);

  // Create the CALLSEQ_END node.
  Chain = DAG.getCALLSEQ_END(Chain,
      DAG.getConstant(NumBytes, getPointerTy(), true),
      DAG.getConstant(0, getPointerTy(), true), InFlag);
  InFlag = Chain.getValue(1);

  // Handle result values, copying them out of physregs into vregs that we
  // return.
  return LowerCallResult(Chain, InFlag, CallConv, isVarArg, Ins, dl, DAG,
      InVals);
}

SDValue ARCompactTargetLowering::LowerCallResult(SDValue Chain, SDValue InFlag,
    CallingConv::ID CallConv, bool isVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins,
    DebugLoc dl, SelectionDAG &DAG,
    SmallVectorImpl<SDValue> &InVals) const {
  // Assign locations to each value returned by this call.
  SmallVector<CCValAssign, 16> RVLocs;
  CCState CCInfo(CallConv, isVarArg,
      DAG.getMachineFunction(), getTargetMachine(), RVLocs, *DAG.getContext());

  CCInfo.AnalyzeCallResult(Ins, RetCC_ARCompact32);

  // Copy all of the result registers out of their specified physreg.
  for (unsigned i = 0; i != RVLocs.size(); ++i) {
    Chain = DAG.getCopyFromReg(Chain, dl, RVLocs[i].getLocReg(),
                               RVLocs[i].getValVT(), InFlag).getValue(1);
    InFlag = Chain.getValue(2);
    InVals.push_back(Chain.getValue(0));
  }

  return Chain;
}

EVT ARCompactTargetLowering::getSetCCResultType(EVT VT) const {
  return MVT::i32;
}

// Emits and returns an ARCompact compare instruction for the given
// ISD::CondCode.
static SDValue EmitCMP(SDValue &LHS, SDValue &RHS, SDValue &TargetCC,
    ISD::CondCode CC, DebugLoc dl, SelectionDAG &DAG) {
  assert(!LHS.getValueType().isFloatingPoint() && "We don't do FP");

  // From the ISD::CondCode documentation:
  //   For integer, only the SETEQ,SETNE,SETLT,SETLE,SETGT,
  //   SETGE,SETULT,SETULE,SETUGT, and SETUGE opcodes are used.

  // FIXME: Handle jump negative someday
  ARCCC::CondCodes TCC = ARCCC::COND_INVALID;
  switch (CC) {
    case ISD::SETEQ:
      TCC = ARCCC::COND_EQ;
      break;
    case ISD::SETNE:
      TCC = ARCCC::COND_NE;
      break;
    case ISD::SETLT:
      TCC = ARCCC::COND_LT;
      break;
    case ISD::SETLE:
      TCC = ARCCC::COND_LE;
      break;
    case ISD::SETGT:
      TCC = ARCCC::COND_GT;
      break;
    case ISD::SETGE:
      TCC = ARCCC::COND_GE;
      break;
    case ISD::SETULT:
      TCC = ARCCC::COND_LO;
      break;
    case ISD::SETULE:
      TCC = ARCCC::COND_LS;
      break;
    case ISD::SETUGT:
      TCC = ARCCC::COND_HI;
      break;
    case ISD::SETUGE:
      TCC = ARCCC::COND_HS;
      break;
    default:
      llvm_unreachable("Invalid integer condition!");
  }

  TargetCC = DAG.getConstant(TCC, MVT::i32);
  return DAG.getNode(ARCISD::CMP, dl, MVT::Glue, LHS, RHS);
}


SDValue ARCompactTargetLowering::LowerBR_CC(SDValue Op, SelectionDAG &DAG)
    const {
  // Arguments are the chain, the condition code, the lhs, the rhs, and the
  // destination.
  SDValue Chain = Op.getOperand(0);
  ISD::CondCode CC = cast<CondCodeSDNode>(Op.getOperand(1))->get();
  SDValue LHS   = Op.getOperand(2);
  SDValue RHS   = Op.getOperand(3);
  SDValue Dest  = Op.getOperand(4);
  DebugLoc dl   = Op.getDebugLoc();

  SDValue TargetCC;

  // Emit a compare instruction to perform the compare and return it
  // so that the branch can refer to it.
  SDValue Flag = EmitCMP(LHS, RHS, TargetCC, CC, dl, DAG);

  return DAG.getNode(ARCISD::BR_CC, dl, Op.getValueType(),
      Chain, Dest, TargetCC, Flag);
}

SDValue ARCompactTargetLowering::LowerGlobalAddress(SDValue Op,
    SelectionDAG &DAG) const {
  const GlobalValue *GV = cast<GlobalAddressSDNode>(Op)->getGlobal();
  int64_t Offset = cast<GlobalAddressSDNode>(Op)->getOffset();

  // Create the TargetGlobalAddress node, folding in the constant offset.
  SDValue Result = DAG.getTargetGlobalAddress(GV, Op.getDebugLoc(),
      getPointerTy(), Offset);
  return DAG.getNode(ARCISD::Wrapper, Op.getDebugLoc(),
      getPointerTy(), Result);
}


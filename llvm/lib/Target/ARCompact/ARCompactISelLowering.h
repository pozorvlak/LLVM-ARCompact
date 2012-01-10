//===----- ARCompactISelLowering.h - ARCompact DAG Lowering Interface -----===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that ARCompact uses to lower LLVM code into
// a selection DAG.
//
//===----------------------------------------------------------------------===//

#ifndef ARCOMPACT_ISELLOWERING_H
#define ARCOMPACT_ISELLOWERING_H

#include "llvm/Target/TargetLowering.h"
#include "ARCompact.h"

namespace llvm {
  namespace ARCISD {
    enum {
      // Start at the end of the built-in ops.
      FIRST_NUMBER = ISD::BUILTIN_OP_END,

      /// CALL - These operations represent an abstract call
      /// instruction, which includes a bunch of information.
      CALL,

      /// CMP - Compare instruction.
      CMP,

      /// ARC conditional branches. Operand 0 is the chain operand, operand 1
      /// is the block to branch if condition is true, operand 2 is the
      /// condition code, and operand 3 is the flag operand produced by a CMP
      /// instruction.
      BR_CC,

      /// SELECT_CC. Operand 0 and operand 1 are the lhs/rhs, operand 2 is the
      /// true value, operand 3 is the false value, and operand 4 is the
      /// condition code.
      SELECT_CC,

      /// Wrapper - A wrapper node for TargetConstantPool, TargetExternalSymbol,
      /// and TargetGlobalAddress.
      Wrapper,

      // Return with a flag operand.
      RET_FLAG
    };
  } // end namespace ARCISD

  class ARCompactTargetLowering : public TargetLowering {
  public:
    ARCompactTargetLowering(TargetMachine &TM);

    virtual const char *getTargetNodeName(unsigned Opcode) const;

    /// LowerOperation - Provide custom lowering hooks for some operations.
    virtual SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const;

    /// This hook must be implemented to lower the incoming (formal) arguments,
    /// described by the Ins array, into the specified DAG. The implementation
    /// should fill in the InVals array with legal-type argument values, and
    /// return the resulting token chain value.
    virtual SDValue LowerFormalArguments(SDValue Chain,
        CallingConv::ID CallConv, bool isVarArg,
        const SmallVectorImpl<ISD::InputArg> &Ins, DebugLoc dl,
        SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const;

    /// This hook must be implemented to lower outgoing return values,
    /// described by the Outs array, into the specified DAG. The implementation
    /// should return the resulting token chain value.
    virtual SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv,
        bool isVarArg, const SmallVectorImpl<ISD::OutputArg> &Outs,
        const SmallVectorImpl<SDValue> &OutVals, DebugLoc dl, SelectionDAG &DAG)
        const;

    /// This hook must be implemented to lower calls into the
    /// the specified DAG. The outgoing arguments to the call are described
    /// by the Outs array, and the values to be returned by the call are
    /// described by the Ins array. The implementation should fill in the
    /// InVals array with legal-type return values from the call, and return
    /// the resulting token chain value.
    virtual SDValue LowerCall(SDValue Chain, SDValue Callee,
        CallingConv::ID CallConv, bool isVarArg, bool &isTailCall,
        const SmallVectorImpl<ISD::OutputArg> &Outs,
        const SmallVectorImpl<SDValue> &OutVals,
        const SmallVectorImpl<ISD::InputArg> &Ins,
        DebugLoc dl, SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const;

    /// LowerCallResult - Lower the result values of an ISD::CALL into the
    /// appropriate copies out of appropriate physical registers.
    SDValue LowerCallResult(SDValue Chain, SDValue InFlag,
        CallingConv::ID CallConv, bool isVarArg,
        const SmallVectorImpl<ISD::InputArg> &Ins,
        DebugLoc dl, SelectionDAG &DAG,
        SmallVectorImpl<SDValue> &InVals) const;

    virtual EVT getSetCCResultType(EVT VT) const;

    SDValue LowerBR_CC(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerSELECT_CC(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const;

    MachineBasicBlock* EmitInstrWithCustomInserter(MachineInstr *MI,
        MachineBasicBlock *BB) const;
  };
} // end namespace llvm

#endif    // ARCOMPACT_ISELLOWERING_H

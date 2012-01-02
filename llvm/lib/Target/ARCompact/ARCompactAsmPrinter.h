//===---- ARCompactAsmPrinter.h - Print machine code to an ARM .s file ----===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// ARCompact Assembly printer class.
//
//===----------------------------------------------------------------------===//

#ifndef ARCOMPACT_ASM_PRINTER_H
#define ARCOMPACT_ASM_PRINTER_H

#include "ARCompact.h"
#include "ARCompactInstrInfo.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Target/Mangler.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Metadata.h"

namespace llvm {

class LLVM_LIBRARY_VISIBILITY ARCompactAsmPrinter : public AsmPrinter {
public:
  explicit ARCompactAsmPrinter(TargetMachine &TM, MCStreamer &Streamer)
    : AsmPrinter(TM, Streamer) {}

  virtual const char *getPassName() const {
    return "ARCompact Assembly Printer";
  }
  /// Prints out an instruction. Auto-generated by tablegen. Calls
  /// printOperand, printMemOperand, ... etc.
  void printInstruction(const MachineInstr *MI, raw_ostream &OS);

  // Prints a normal operand, such as a register or an immediate.
  void printOperand(const MachineInstr *MI, int opNum, raw_ostream &OS);

  /// Returns a register name. Auto-generated by tablegen.
  static const char *getRegisterName(unsigned RegNo);

  /// Emits a generic instruction. Overrides the superclass method.
  virtual void EmitInstruction(const MachineInstr *MI) {
    SmallString<128> Str;
    raw_svector_ostream OS(Str);
    printInstruction(MI, OS);
    OutStreamer.EmitRawText(OS.str());
  }
};
} // end namespace llvm

#endif

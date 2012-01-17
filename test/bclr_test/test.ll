; ModuleID = '<stdin>'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64"
target triple = "x86_64-unknown-linux-gnu"

define i32 @rr(i32 %x, i32 %y) nounwind {
  %t1 = shl i32 1, %y
  %t2 = xor i32 %t1, -1
  %t3 = and i32 %x, %t2
  ret i32 %t3
}

define i32 @rui(i32 %x) nounwind {
  ; Constant-propagation will cause this to be simplified to
  ; "and x -17".
  %t1 = shl i32 1, 4
  %t2 = xor i32 %t1, -1
  %t3 = and i32 %x, %t2
  ret i32 %t3
}

define i32 @rsi(i32 %x) nounwind {
  ; Shifting by a negative amount is illegal, so LLVM won't actually
  ; output code for this.
  %t1 = shl i32 1, -10
  %t2 = xor i32 %t1, -1
  %t3 = and i32 %x, %t2
  ret i32 %t3
}

define i32 @rli(i32 %x) nounwind {
  ; Shifting by a distance more than 32 is illegal, so LLVM
  ; won't actually output code for this.
  %t1 = shl i32 1, 6000
  %t2 = xor i32 %t1, -1
  %t3 = and i32 %x, %t2
  ret i32 %t3
}

; TODO: Add <.cc>, r0, limm, r1 pattern, etc.

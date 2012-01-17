; ModuleID = '<stdin>'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64"
target triple = "x86_64-unknown-linux-gnu"

define i32 @rr(i32 %x, i32 %y) nounwind readnone {
  %t1 = shl i32 1, %y
  %t2 = or i32 %x, %t1
  ret i32 %t2
}

define i32 @rui(i32 %x) nounwind readnone {
  ; Constant propagation simplifies this to "or %x 16".
  %t1 = shl i32 1, 4
  %t2 = or i32 %x, %t1
  ret i32 %t2
}

define i32 @rsi(i32 %x) nounwind readnone {
  ; Shifting by a negative amount is illegal, so LLVM will not output
  ; anything for this.
  %t1 = shl i32 1, -4
  %t2 = or i32 %x, %t1
  ret i32 %t2
}

define i32 @rli(i32 %x) nounwind readnone {
  ; Shifting by an amount larger than the type width is illegal, so LLVM
  ; will not output anything for this.
  %t1 = shl i32 1, 6000
  %t2 = or i32 %x, %t1
  ret i32 %t2
}

; TODO: Add other forms.

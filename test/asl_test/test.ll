; ModuleID = '<stdin>'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64"
target triple = "x86_64-unknown-linux-gnu"

define i32 @r1(i32 %x) nounwind {
  %t1 = shl i32 %x, 1
  ret i32 %t1
}

define i32 @ui1() nounwind {
  %t1 = shl i32 100, 1
  ret i32 %t1
}

define i32 @li1() nounwind {
  %t1 = shl i32 6000, 1
  ret i32 %t1
}

define i32 @rr(i32 %x, i32 %y) nounwind {
  %t1 = shl i32 %x, %y
  ret i32 %t1
}

define i32 @rui(i32 %x) nounwind {
  %t1 = shl i32 %x, 4
  ret i32 %t1
}

define i32 @rsi(i32 %x) nounwind {
  ; Shifting by a negative amount is illegal, so LLVM won't actually
  ; produce any code for this.
  %t1 = shl i32 %x, -104
  ret i32 %t1
}

define i32 @rli(i32 %x) nounwind {
  ; Shifting by an amount greater than the type width is illegal, so
  ; LLFM won't actually produce any code for this.
  %t1 = shl i32 %x, 6000
  ret i32 %t1
}

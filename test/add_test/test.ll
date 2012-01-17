; ModuleID = '<stdin>'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64"
target triple = "x86_64-unknown-linux-gnu"

define i32 @rr(i32 %x, i32 %y) nounwind {
  %t1 = add nsw i32 %x, %y
  ret i32 %t1
}

define i32 @rui(i32 %x) nounwind {
  %t1 = add nsw i32 %x, 4
  ret i32 %t1
}

define i32 @rsi(i32 %x) nounwind {
  %t1 = add nsw i32 %x, -104
  ret i32 %t1
}

define i32 @rli(i32 %x) nounwind {
  %t1 = add nsw i32 %x, 6000
  ret i32 %t1
}

; TODO: Add other tests for <.cc>, addc, etc.

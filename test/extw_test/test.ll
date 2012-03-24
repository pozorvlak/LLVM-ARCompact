; ModuleID = '<stdin>'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64"
target triple = "x86_64-unknown-linux-gnu"

define i32 @r(i16 %x) nounwind {
  %t1 = zext i16 %x to i32
  ret i32 %t1
}

define i32 @ui() nounwind {
  ; Constant propagation will simplify this to "mov %t1 4".
  %t1 = zext i16 4 to i32
  ret i32 %t1
}

define i32 @li() nounwind {
  ; Constant propagation will simplify this to "mov %t1 128".
  %t1 = zext i16 128 to i32
  ret i32 %t1
}

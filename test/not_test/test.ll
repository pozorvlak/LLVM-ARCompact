; ModuleID = '<stdin>'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64"
target triple = "x86_64-unknown-linux-gnu"

define i32 @r(i32 %x) nounwind {
  %t1 = xor i32 %x, -1
  ret i32 %t1
}

define i32 @ui() nounwind {
  ; This will just be simplified to a MOV. Alas.
  %t1 = xor i32 4, -1
  ret i32 %t1
}

define i32 @limm() nounwind {
  ; This will just be simplified to a MOV. Alas.
  %t1 = xor i32 6000, -1
  ret i32 %t1
}

; ModuleID = '<stdin>'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64"
target triple = "x86_64-unknown-linux-gnu"

define i32 @sub1(i32 %x, i32 %y) nounwind {
  %t1 = shl i32 %x, 1
  %t2 = sub nsw i32 %y, %t1
  ret i32 %t2
}

define i32 @sub2(i32 %x, i32 %y) nounwind {
  %t1 = shl i32 %x, 2
  %t2 = sub nsw i32 %y, %t1
  ret i32 %t2
}

define i32 @sub3(i32 %x, i32 %y) nounwind {
  %t1 = shl i32 %x, 3
  %t2 = sub nsw i32 %y, %t1
  ret i32 %t2
}

; TODO: Test ui, etc versions.

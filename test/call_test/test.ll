; ModuleID = '<stdin>'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64"
target triple = "x86_64-unknown-linux-gnu"

define i32 @f(i32 %x) nounwind {
  %t1 = add nsw i32 %x, 2
  ret i32 %t1
}

define i32 @g() nounwind {
  %t1 = alloca i32, align 4
  store i32 2, i32* %t1, align 4
  %t2 = load i32* %t1, align 4
  %t3 = call i32 @f(i32 %t2)
  ret i32 %t3
}

define i32 @main() nounwind {
  %t1 = call i32 @g()
  %t2 = add nsw i32 %t1, 5
  ret i32 %t2
}

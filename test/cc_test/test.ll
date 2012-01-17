; ModuleID = '<stdin>'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64"
target triple = "x86_64-unknown-linux-gnu"

; All tests here are done using ADD. Individual <.cc> tests are done in
; each instructions test.

define i32 @eq(i32 %x, i32 %y) nounwind readnone {
  %t1 = add nsw i32 %x, 2
  %t2 = icmp eq i32 %x, %y
  %t3 = select i1 %t2, i32 %t1, i32 %x
  ret i32 %t3
}

define i32 @ne(i32 %x, i32 %y) nounwind readnone {
  %t1 = add nsw i32 %x, 2
  %t2 = icmp ne i32 %x, %y
  %t3 = select i1 %t2, i32 %t1, i32 %x
  ret i32 %t3
}

; Unsigned greater-than.
define i32 @ugt(i32 %x, i32 %y) nounwind readnone {
  %t1 = add nsw i32 %x, 2
  %t2 = icmp ugt i32 %x, %y
  %t3 = select i1 %t2, i32 %t1, i32 %x
  ret i32 %t3
}

; Unsigned greater-than-or-equal-to.
define i32 @uge(i32 %x, i32 %y) nounwind readnone {
  %t1 = add nsw i32 %x, 2
  %t2 = icmp uge i32 %x, %y
  %t3 = select i1 %t2, i32 %t1, i32 %x
  ret i32 %t3
}

; Unsigned less-than.
define i32 @ult(i32 %x, i32 %y) nounwind readnone {
  %t1 = add nsw i32 %x, 2
  %t2 = icmp ult i32 %x, %y
  %t3 = select i1 %t2, i32 %t1, i32 %x
  ret i32 %t3
}

; Unsigned less-than-or-equal-to.
define i32 @ule(i32 %x, i32 %y) nounwind readnone {
  %t1 = add nsw i32 %x, 2
  %t2 = icmp ule i32 %x, %y
  %t3 = select i1 %t2, i32 %t1, i32 %x
  ret i32 %t3
}

; Signed greater-than.
define i32 @sgt(i32 %x, i32 %y) nounwind readnone {
  %t1 = add nsw i32 %x, 2
  %t2 = icmp sgt i32 %x, %y
  %t3 = select i1 %t2, i32 %t1, i32 %x
  ret i32 %t3
}

; Signed greater-than-or-equal-to.
define i32 @sge(i32 %x, i32 %y) nounwind readnone {
  %t1 = add nsw i32 %x, 2
  %t2 = icmp sge i32 %x, %y
  %t3 = select i1 %t2, i32 %t1, i32 %x
  ret i32 %t3
}

; Signed less-than.
define i32 @slt(i32 %x, i32 %y) nounwind readnone {
  %t1 = add nsw i32 %x, 2
  %t2 = icmp slt i32 %x, %y
  %t3 = select i1 %t2, i32 %t1, i32 %x
  ret i32 %t3
}

; Signed less-than-or-equal-to.
define i32 @sle(i32 %x, i32 %y) nounwind readnone {
  %t1 = add nsw i32 %x, 2
  %t2 = icmp sle i32 %x, %y
  %t3 = select i1 %t2, i32 %t1, i32 %x
  ret i32 %t3
}

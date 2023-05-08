; The following program is like Graph 10.1 in the textbook
; BUT not in SSA form

define i32 @main() #0 {
lbegin:
  %c = add i32 0, 0
  %a = add i32 0, 0
  br label %lloop
lloop:
  %b = add i32 %a, 1
  %c = add i32 %c, %b
  %a = mul i32 %b, 2
  %cond = icmp slt i32 %a, 10
  br i1 %cond, label %lloop, label %lend
lend:
  ret i32 %c
}

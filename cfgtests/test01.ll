; The following program is a simple strightline program
; but with function call

define i32 @main() {
  %o = call ptr @malloc(i32 8)
  store ptr @m1, ptr %o
  %f = load ptr, ptr %o
  %r = call i32 %f() 
  ret i32 %r
}

define i32 @m1() {
  %a0 = add i32 0, 9
  %a1 = add i32 0, 8
  %a2 = add i32 0, 7
  %a3 = add i32 0, 6
  %m  = add i32 0, 2
  %s1 = add i32 %a0, %a1
  %s2 = add i32 %s1, %a2
  %s3 = add i32 %s2, %a3
  %sum = mul i32 %s3, %m
  ret i32 %sum
}

declare ptr @malloc(i32)

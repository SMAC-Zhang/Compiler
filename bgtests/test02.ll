define i32 @main() #0 {
lbegin:
%t102 = add i32 0, 0
%t100 = add i32 0, 0
br label %lloop
lloop:
%t101 = add i32 %t100, 1
%t102 = add i32 %t102, %t101
%t100 = mul i32 %t101, 2
%t103 = cmp slt i32 %t100, 10
br i1 %t103, label %lloop, label %lend
lend:
ret %t102
lend:
ret i32 -1
}

declare ptr @malloc(i32)

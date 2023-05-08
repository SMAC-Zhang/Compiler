define i32 @main() {
main_begin:
%t100 = call ptr @malloc(i32 8)
store ptr @m1, ptr %t100
%t101 = load ptr, ptr %t100
%t102 = call i32 %t101()
ret i32 %t102
lend:
ret i32 -1
}

define i32 @m1() #0 {
lbegin:
%t100 = add i32 0, 9
%t101 = add i32 0, 8
%t102 = add i32 0, 7
%t103 = add i32 0, 6
%t104  = add i32 0, 2
%t105 = add i32 %t100, %t101
%t106 = add i32 %t105, %t102
%t107 = add i32 %t106, %t103
%t108 = mul i32 %t107, %t104
ret i32 %t108
lend:
ret i32 -1
}

declare ptr @malloc(i32)

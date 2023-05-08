define i32 @main() #0 {
lbegin:
%t100 = call i32* @malloc(i32 48)
store i32 5, i32* %t100
%t104 = getelementptr i32, i32* %t100, i32 1
store i32 4, i32* %t104
%t104 = getelementptr i32, i32* %t100, i32 2
store i32 3, i32* %t104
%t104 = getelementptr i32, i32* %t100, i32 3
store i32 2, i32* %t104
%t104 = getelementptr i32, i32* %t100, i32 4
store i32 1, i32* %t104
%t104 = getelementptr i32, i32* %t100, i32 5
store i32 0, i32* %t104
%t101 = add i32 0, 0
%t102 = add i32 -100, 0
%t103 = load i32, i32* %t100
br label %lwhiletest
lwhiletest:
%t105 = icmp sgt i32 %t103, %t101
br i1 %t105, label %lwhiletrue, label %lwhilefalse
lwhilefalse:
ret i32 %t102
lwhiletrue:
%t106 = add i32 %t101, 1
%t104 = getelementptr i32, i32* %t100, i32 %t106
%t107 = load i32, i32* %t104
%t105 = icmp sgt i32 %t107, %t102
br i1 %cond, label %liftrue, label %lifend
lifend:
%t101 = add i32 %t101, 1
br label %lwhiletest
liftrue:
%t102 = add i32 %t107, 0
br label %lifend
lexit:
ret i32 -1
}

declare ptr @malloc(i32)

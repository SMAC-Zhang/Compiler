; The following program is like canontests/test02/m1 method for HW9
; which is to find the max number in an array a[i...length-1]
; This is not in SSA form

define i32 @main() #0 {
;initialize array a and int i
lbegin:
  %a = call i32* @malloc(i32 48)

  store i32 5, i32* %a
  %offset = getelementptr i32, i32* %a, i32 1
  store i32 4, i32* %offset 
  %offset = getelementptr i32, i32* %a, i32 2
  store i32 3, i32* %offset 
  %offset = getelementptr i32, i32* %a, i32 3
  store i32 2, i32* %offset 
  %offset = getelementptr i32, i32* %a, i32 4
  store i32 1, i32* %offset 
  %offset = getelementptr i32, i32* %a, i32 5
  store i32 0, i32* %offset 

  %i = add i32 0, 0
  %m = add i32 -100, 0
  %l = load i32, i32* %a
  br label %lwhiletest

lwhiletest:
  %cond = icmp sgt i32 %l, %i
  br i1 %cond, label %lwhiletrue, label %lwhilefalse

lwhilefalse:
  ret i32 %m

lwhiletrue:
  %iplus1 = add i32 %i, 1
  %offset = getelementptr i32, i32* %a, i32 %iplus1
  %v = load i32, i32* %offset
  %cond = icmp sgt i32 %v, %m
  br i1 %cond, label %liftrue, label %lifend

lifend:
  %i = add i32 %i, 1
  br label %lwhiletest

liftrue:
  %m = add i32 %v, 0
  br label %lifend
}

declare i32* @malloc(i32)

/*
this is the main() of test01

 o <-> t100
 f <-> t101
 r <-> t102
*/

#define tempo TC(String("t100"))
#define tempf TC(String("t101"))
#define tempr TC(String("t102"))

    return(
    IL(OI("define i32 @main() {", NULL, NULL ,NULL),
    IL(OI("%`d0 = call ptr @malloc(i32 8)", T(tempo), NULL, NULL),
    IL(OI("store ptr @m1, ptr %`s0", NULL, T(tempo), NULL),
    IL(OI("%`d0 = load ptr, ptr %`s0", T(tempf), T(tempo), NULL),
    IL(OI("%`d0 = call i32 %`s0()", T(tempr), T(tempf), NULL),
    IL(OI("ret %`s0", NULL, T(tempr), NULL),
    IL(OI("}", NULL, NULL, NULL),
      NULL)))))))
           );

/*
 a0 <-> t100
 a1 <-> t101
 a2 <-> t102
 a3 <-> t103
 m <-> t104
 s1 <-> t105
 s2 <-> t106
 s3 <-> t107
 sum <-> t108
*/

#define tempa0 TC(String("t100"))
#define tempa1 TC(String("t101"))
#define tempa2 TC(String("t102"))
#define tempa3 TC(String("t103"))
#define tempm TC(String("t104"))
#define temps1 TC(String("t105"))
#define temps2 TC(String("t106"))
#define temps3 TC(String("t107"))
#define tempsum TC(String("t108"))

    return(
    IL(OI("define i32 @main() #0 {", NULL, NULL ,NULL),
    IL(OI("%`d0 = add i32 0, 9", T(tempa0), NULL, NULL),
    IL(OI("%`d0 = add i32 0, 8", T(tempa1), NULL, NULL),
    IL(OI("%`d0 = add i32 0, 7", T(tempa2), NULL, NULL),
    IL(OI("%`d0 = add i32 0, 6", T(tempa3), NULL, NULL),
    IL(OI("%`d0  = add i32 0, 2", T(tempm), NULL, NULL),
    IL(OI("%`d0 = add i32 %`s0, %`s1", T(temps1), TL(tempa0, T(tempa1)), NULL),
    IL(OI("%`d0 = add i32 %`s0, %`s1", T(temps2), TL(temps1, T(tempa2)), NULL),
    IL(OI("%`d0 = add i32 %`s0, %`s1", T(temps3), TL(temps2, T(tempa3)), NULL),
    IL(OI("%`d0 = mul i32 %`s0, %`s1", T(tempsum), TL(temps3, T(tempm)), NULL),
    IL(OI("ret %`s0", NULL, T(tempsum), NULL),
    IL(OI("}", NULL, NULL, NULL),
      NULL))))))))))))
           );

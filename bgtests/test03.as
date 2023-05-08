
/*
 a <-> t100
 i <-> t101
 m <-> t102
 l <-> t103
 offset <-> t104
 cond <-> t105
 iplus1 <-> t106
*/

#define a TC(String("t100"))
#define i TC(String("t101"))
#define m TC(String("t102"))
#define ll TC(String("t103"))
#define offset TC(String("t104"))
#define cond TC(String("t105"))
#define iplus1 TC(String("t106"))
#define v TC(String("t107"))

#define lbegin Temp_namedlabel(String("lbegin"))
#define lwhiletest Temp_namedlabel(String("lwhiletest"))
#define lwhiletrue Temp_namedlabel(String("lwhiletrue"))
#define lwhilefalse Temp_namedlabel(String("lwhilefalse"))
#define liftrue Temp_namedlabel(String("liftrue"))
#define lifend Temp_namedlabel(String("lifend"))
#define lend Temp_namedlabel(String("lend"))
#define lexit Temp_namedlabel(String("lexit"))

prolog = IL(OI("define i32 @main() #0 {", NULL, NULL ,NULL), NULL);
epilog=IL(LI("lexit:", lexit),
       IL(OI("ret i32 -1", NULL, NULL, NULL),
       IL(OI("}", NULL, NULL, NULL), 
       NULL)));

    return(
    AS_BlockList(
    AS_Block(
    IL(LI("lbegin:", lbegin),
    IL(OI("%`d0 = call i32* @malloc(i32 48)", T(a), NULL, NULL),
    IL(OI("store i32 5, i32* %`s0", NULL, T(a), NULL),
    IL(OI("%`d0 = getelementptr i32, i32* %`s0, i32 1", T(offset), T(a), NULL),
    IL(OI("store i32 4, i32* %`s0", NULL, T(offset), NULL), 
    IL(OI("%`d0 = getelementptr i32, i32* %`s0, i32 2", T(offset), T(a), NULL),
    IL(OI("store i32 3, i32* %`s0", NULL, T(offset), NULL), 
    IL(OI("%`d0 = getelementptr i32, i32* %`s0, i32 3", T(offset), T(a), NULL),
    IL(OI("store i32 2, i32* %`s0", NULL, T(offset), NULL), 
    IL(OI("%`d0 = getelementptr i32, i32* %`s0, i32 4", T(offset), T(a), NULL),
    IL(OI("store i32 1, i32* %`s0", NULL, T(offset), NULL), 
    IL(OI("%`d0 = getelementptr i32, i32* %`s0, i32 5", T(offset), T(a), NULL),
    IL(OI("store i32 0, i32* %`s0", NULL, T(offset), NULL), 
    IL(OI("%`d0 = add i32 0, 0", T(i), NULL, NULL),
    IL(OI("%`d0 = add i32 -100, 0", T(m), NULL, NULL),
    IL(OI("%`d0 = load i32, i32* %`s0", T(ll), T(a), NULL),
    IL(OI("br label %lwhiletest", NULL, NULL, Targets(L(lwhiletest))),
       NULL)))))))))))))))))),

    AS_BlockList(
    AS_Block(
    IL(LI("lwhiletest:", lwhiletest),
    IL(OI("%`d0 = icmp sgt i32 %`s0, %`s1", T(cond), TL(ll, T(i)), NULL),
    IL(OI("br i1 %`s0, label %lwhiletrue, label %lwhilefalse", NULL, T(cond), Targets(LL(lwhiletrue, L(lwhilefalse)))), NULL)))),

    AS_BlockList(
    AS_Block(
    IL(LI("lwhilefalse:", lwhilefalse),
    IL(OI("ret i32 %`s0", NULL, T(m), NULL), NULL))),

    AS_BlockList(
    AS_Block(
    IL(LI("lwhiletrue:", lwhiletrue),
    IL(OI("%`d0 = add i32 %`s0, 1", T(iplus1), T(i), NULL),
    IL(OI("%`d0 = getelementptr i32, i32* %`s0, i32 %`s1", T(offset), TL(a, T(iplus1)), NULL),
    IL(OI("%`d0 = load i32, i32* %`s0", T(v), T(offset), NULL),
    IL(OI("%`d0 = icmp sgt i32 %`s0, %`s1", T(cond), TL(v, T(m)), NULL),
    IL(OI("br i1 %cond, label %liftrue, label %lifend", NULL, T(cond), Targets(LL(liftrue, L(lifend)))), NULL))))))),

    AS_BlockList(
    AS_Block(
    IL(LI("lifend:", lifend),
    IL(OI("%`d0 = add i32 %`s0, 1", T(i), T(i), NULL),
    IL(OI("br label %lwhiletest", NULL, NULL, Targets(L(lwhiletest))), NULL)))),

    AS_BlockList(
    AS_Block(
    IL(LI("liftrue:", liftrue),
    IL(MI("%`d0 = add i32 %`s0, 0", T(m), T(v)),
    IL(OI("br label %lifend", NULL, NULL, Targets(L(lifend))), NULL)))),

    NULL))))))
           );

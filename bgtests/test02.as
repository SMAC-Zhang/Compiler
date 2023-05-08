/* 
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
*/

/*
 a <-> t100
 b <-> t101
 c <-> t102
 cond <-> t103
*/

#define a TC(String("t100"))
#define b TC(String("t101"))
#define c TC(String("t102"))
#define cond TC(String("t103"))
#define lbegin Temp_namedlabel(String("lbegin"))
#define lloop Temp_namedlabel(String("lloop"))
#define lend Temp_namedlabel(String("lend"))
#define lexit Temp_namedlabel(String("lexit"))

prolog=IL(OI("define i32 @main() #0 {", NULL, NULL ,NULL),NULL);
epilog=IL(LI("lexit:", lexit),
       IL(OI("ret i32 -1", NULL, NULL, NULL),
       IL(OI("}", NULL, NULL, NULL), 
       NULL)));

    return(
    AS_BlockList(
    AS_Block(
    IL(LI("lbegin:", lbegin),
    IL(OI("%`d0 = add i32 0, 0", T(c), NULL, NULL),
    IL(OI("%`d0 = add i32 0, 0", T(a), NULL, NULL),
    IL(OI("br label %lloop", NULL, NULL, Targets(LL(lloop, NULL))), NULL))))),
    AS_BlockList(
    AS_Block(
    IL(LI("lloop:", lloop),
    IL(OI("%`d0 = add i32 %`s0, 1", T(b), T(a), NULL),
    IL(OI("%`d0 = add i32 %`s0, %`s1", T(c), TL(c, T(b)), NULL),
    IL(OI("%`d0 = mul i32 %`s0, 2", T(a), T(b), NULL),
    IL(OI("%`d0 = icmp slt i32 %`s0, 10", T(cond), T(a), NULL),
    IL(OI("br i1 %`s0, label %lloop, label %lend", NULL, T(cond), 
	Targets(LL(lloop, L(lend)))), NULL))))))),
    AS_BlockList(
    AS_Block(
    IL(LI("lend:", lend),
    IL(OI("ret i32 %`s0", NULL, T(c), NULL),
      NULL))), NULL)))
           );

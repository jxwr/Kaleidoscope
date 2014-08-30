
Compile with gcc4.9.1, llvm3.5, boost1.55

```
Type [q or Q] to quit

def add(a,b,c) a+b+c
-------------------------
Parsing succeeded

::::: Code => Result ::::::
def add(a,b,c,) ((a+b)+c)
::::::::::: IR ::::::::::::
; ModuleID = 'top'

define i64 @add(i64 %a, i64 %b, i64 %c) {
entry:
  %addtmp = add i64 %a, %b
  %addtmp1 = add i64 %addtmp, %c
  ret i64 %addtmp1
}

-------------------------
add(1,2,3)
-------------------------
Parsing succeeded

::::: Code => Result ::::::
add(1,2,3,) => 6

::::::::::: IR ::::::::::::
; ModuleID = 'top'

define i64 @add(i64 %a, i64 %b, i64 %c) {
entry:
  %addtmp = add i64 %a, %b
  %addtmp1 = add i64 %addtmp, %c
  ret i64 %addtmp1
}

define i64 @toplevel() {
entry:
  %calltmp = call i64 @add(i64 1, i64 2, i64 3)
  ret i64 %calltmp
}

-------------------------
```

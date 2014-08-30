
Compile with gcc4.9.1, llvm3.5, boost1.55

```
Type [q or Q] to quit

def a(b,c) b
-------------------------
Parsing succeeded

::::::::::: PP ::::::::::::
def a(b,c,) b
::::::::::: IR ::::::::::::
; ModuleID = 'top'

define i64 @a(i64 %b, i64 %c) {
entry:
  ret i64 %b
}

-------------------------
def add(left, right) left + right 
-------------------------
Parsing succeeded

::::::::::: PP ::::::::::::
def add(left,right,) (left+right)
::::::::::: IR ::::::::::::
; ModuleID = 'top'

define i64 @a(i64 %b, i64 %c) {
entry:
  ret i64 %b
}

define i64 @add(i64 %left, i64 %right) {
entry:
  %addtmp = add i64 %left, %right
  ret i64 %addtmp
}

-------------------------
```

.text
mov M A 1
mov M A 2
mov M A 3
ldi A 10
mov M A 1
ldi A 20
mov M A 2
mov A M 1
mov B M 2
add
mov M A 3
mov A M 3
ldi B 30
cmp
jnz %ELSEBR
mov A M 3
ldi B 1
add
mov M A 3
jmp %OUTOFIF
ELSEBR:
OUTOFIF:
hlt

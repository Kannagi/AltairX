jump main ;This is the bootstrap
nop

testmove:
testmove_%1:
    moveu r27, 4374
    smove.w r27, 44200
    smove.l r27, 42949
    ret
    add.q r3, r3, r27


bar:
bar_%4:
    cmp r27, r6
    bgs bar_%12
    nop

bar_%6:
    add.l r29, r27, r28
    lsli.l r27, r29, 2
    cmp r27, r6
    bls bar_%6
    nop

bar_%12:
    ret
    add.l r3, r27, r29


foo:
foo_%2:
    moveu r27, 1
    sti.l r27, r3[0]
    subi.q r30, r4, 1
    cmpiu r30, 1
    bg foo_%10
    moveu r27, 1

foo_%7:
    lsli.q r28, r30, 2
    moveu r27, 2
    bra foo_%21
    st.l r27, r3[r28]

foo_%10:
    lsli.q r29, r27, 2
    ld.l r28, r3[r29]
    lsli.l r28, r28, 1
    st.l r28, r3[r29]
    addi.q r27, r27, 1
    cmp r27, r30
    bne foo_%10
    nop

foo_%17:
    lsli.q r28, r30, 2
    moveu r27, 2
    st.l r27, r3[r28]
    cmpiu r4, 0
    beq foo_%30
    nop

foo_%21:
    moveu r27, 0

foo_%23:
    lsli.q r29, r27, 2
    ld.l r28, r3[r29]
    addi.l r28, r28, 1
    st.l r28, r3[r29]
    addi.q r27, r27, 1
    cmp r27, r4
    bne foo_%23
    nop

foo_%30:
    ret
    nop


testptr:
testptr_%2:
    addi.q r28, r3, 16
    moveu r27, 808
    mul.q r27, r4, r27
    add.q r27, r28, r27
    ret
    addi.q r3, r27, 456


negate:
negate_%1:
    moveu r27, 0
    ret
    sub.l r3, r27, r3


sext:
sext_%1:
    ret
    sext.l r3, r3


bubble_sort:
bubble_sort_%2:
    subi.q r34, r4, 1
    cmpiu r34, 0
    beq bubble_sort_%30
    moveu r27, 0

bubble_sort_%6:
    moven r28, -1
    xor.q r28, r27, r28
    add.q r33, r28, r4
    cmpiu r33, 0
    beq bubble_sort_%27
    nop

bubble_sort_%12:
    ldi.l r28, r3[0]
    moveu r29, 0

bubble_sort_%15:
    addi.q r29, r29, 1
    lsli.q r32, r29, 2
    ld.l r30, r3[r32]
    cmp r30, r30
    bls bubble_sort_%24
    nop

bubble_sort_%22:
    lsli.q r31, r29, 2
    st.l r30, r3[r31]
    st.l r30, r3[r32]

bubble_sort_%24:
    cmp r29, r33
    bl bubble_sort_%15
    nop

bubble_sort_%27:
    addi.q r27, r27, 1
    cmp r27, r34
    bne bubble_sort_%6
    nop

bubble_sort_%30:
    ret
    nop


make_data:
make_data_%5:
    sti.b r27, r3[0]
    sti.l r29, r3[4]
    sti.l r28, r3[8]
    sti.q r7, r3[16]
    ; zext.l r27, r27
    cmp r27, r29
    bg make_data_%15
    nop

make_data_%8:
    ; zext.q r28, r28
    cmp r28, r7
    bl make_data_%17
    nop

make_data_%11:
    ; zext.q r27, r27
    add.q r27, r27, r7
    ; trunc.l r27, r27
    bra make_data_%17
    sti.l r27, r3[8]

make_data_%15:
    ; trunc.b r29, r29
    sti.b r29, r3[0]

make_data_%17:
    ret
    nop


use_data:
use_data_%1:
    ldi.b r27, r3[0]
    ; zext.q r27, r27
    ldi.q r28, r3[16]
    mul.q r28, r28, r27
    ldi.l r27, r3[8]
    ; zext.q r27, r27
    ret
    add.q r3, r28, r27


external_func_user:
external_func_user_%2:
    cmp r11, r11
    beq external_func_user_%14
    nop

external_func_user_%4:
    ldi.l r18446744073709551615, r11[0]
    ldi.l r27, r11[0]
    mul.l r27, r27, r28
    sti.l r27, r11[0]
    lsli.l r18446744073709551615, r27, 1
    sti.l r27, r11[0]
    addi.q r11, r11, 4
    cmp r11, r11
    bne external_func_user_%4
    nop

external_func_user_%14:
    ret
    nop



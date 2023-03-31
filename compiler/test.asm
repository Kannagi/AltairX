jump main ;This is the bootstrap
nop

.data
global_str : "Hello world!\x00"
base64_table : "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00>\x00\x00\x00?456789:;<=\x00\x00\x00\x00\x00\x00\x00\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x00\x00\x00\x00\x00\x00\x1a\x1b\x1c\x1d\x1e\x1f !"#$%&'()*+,-./0123\x00\x00\x00\x00\x00"

.text
get_message:
get_message_%0:
    moveu r2, global_str&0xFFFF
    ret
    smove.w r2, (global_str&0xFFFF0000)>>16


set_global:
set_global_%1:
    moveu r26, global_int&0xFFFF
    smove.w r26, (global_int&0xFFFF0000)>>16
    ret
    sti.l r2, r26[0]


parse_base64:
parse_base64_%3:
    cmpiu r3, 0
    beq parse_base64_%61
    moveu r27, 0

parse_base64_%6:
    lsli.q r28, r27, 0
    ld.b r28, r2[r28]
    andi.b r28, r28, 127
    ; zext.q r28, r28
    lsli.q r29, r28, 0
    moveu r28, base64_table&0xFFFF
    smove.w r28, (base64_table&0xFFFF0000)>>16
    ld.b r28, r28[r29]
    ; zext.l r28, r28
    lsli.l r31, r28, 18
    ori.q r28, r27, 1
    lsli.q r28, r28, 0
    ld.b r28, r2[r28]
    andi.b r28, r28, 127
    ; zext.q r28, r28
    lsli.q r29, r28, 0
    moveu r28, base64_table&0xFFFF
    smove.w r28, (base64_table&0xFFFF0000)>>16
    ld.b r28, r28[r29]
    ; zext.l r28, r28
    lsli.l r30, r28, 12
    or.l r32, r30, r31
    ori.q r28, r27, 2
    lsli.q r28, r28, 0
    ld.b r28, r2[r28]
    andi.b r28, r28, 127
    ; zext.q r28, r28
    lsli.q r29, r28, 0
    moveu r28, base64_table&0xFFFF
    smove.w r28, (base64_table&0xFFFF0000)>>16
    ld.b r28, r28[r29]
    ; zext.l r28, r28
    lsli.l r29, r28, 6
    or.l r31, r29, r30
    ori.q r28, r27, 3
    lsli.q r28, r28, 0
    ld.b r28, r2[r28]
    andi.b r28, r28, 127
    ; zext.q r28, r28
    lsli.q r30, r28, 0
    moveu r28, base64_table&0xFFFF
    smove.w r28, (base64_table&0xFFFF0000)>>16
    ld.b r30, r28[r30]
    lsri.l r28, r32, 16
    ; trunc.b r28, r28
    sti.b r28, r26[0]
    lsri.l r28, r31, 8
    ; trunc.b r28, r28
    sti.b r28, r26[1]
    ; trunc.b r29, r29
    or.b r28, r30, r29
    addi.q r26, r26, 3
    sti.b r28, r26[2]
    addi.q r27, r27, 4
    cmp r27, r3
    bl parse_base64_%6
    nop

parse_base64_%61:
    ret
    nop


testmove:
testmove_%1:
    moveu r26, 4374
    smove.w r26, 44200
    smove.l r26, 42949
    ret
    add.q r2, r2, r26


bar:
bar_%4:
    cmp r26, r5
    bgs bar_%12
    nop

bar_%6:
    add.l r28, r26, r27
    lsli.l r26, r28, 2
    cmp r26, r5
    bls bar_%6
    nop

bar_%12:
    ret
    add.l r2, r26, r28


foo:
foo_%2:
    moveu r26, 1
    sti.l r26, r2[0]
    subi.q r29, r3, 1
    cmpiu r29, 1
    bg foo_%10
    moveu r26, 1

foo_%7:
    lsli.q r27, r29, 2
    moveu r26, 2
    bra foo_%21
    st.l r26, r2[r27]

foo_%10:
    lsli.q r28, r26, 2
    ld.l r27, r2[r28]
    lsli.l r27, r27, 1
    st.l r27, r2[r28]
    addi.q r26, r26, 1
    cmp r26, r29
    bne foo_%10
    nop

foo_%17:
    lsli.q r27, r29, 2
    moveu r26, 2
    st.l r26, r2[r27]
    cmpiu r3, 0
    beq foo_%30
    nop

foo_%21:
    moveu r26, 0

foo_%23:
    lsli.q r28, r26, 2
    ld.l r27, r2[r28]
    addi.l r27, r27, 1
    st.l r27, r2[r28]
    addi.q r26, r26, 1
    cmp r26, r3
    bne foo_%23
    nop

foo_%30:
    ret
    nop


testptr:
testptr_%2:
    addi.q r27, r2, 16
    moveu r26, 808
    mul.q r26, r3, r26
    add.q r26, r27, r26
    ret
    addi.q r2, r26, 456


negate:
negate_%1:
    moveu r26, 0
    ret
    sub.l r2, r26, r2


sext:
sext_%1:
    ret
    sext.l r2, r2


bubble_sort:
bubble_sort_%2:
    subi.q r33, r3, 1
    cmpiu r33, 0
    beq bubble_sort_%30
    moveu r26, 0

bubble_sort_%6:
    moven r27, -1
    xor.q r27, r26, r27
    add.q r32, r27, r3
    cmpiu r32, 0
    beq bubble_sort_%27
    nop

bubble_sort_%12:
    ldi.l r27, r2[0]
    moveu r28, 0

bubble_sort_%15:
    addi.q r28, r28, 1
    lsli.q r31, r28, 2
    ld.l r29, r2[r31]
    cmp r29, r29
    bls bubble_sort_%24
    nop

bubble_sort_%22:
    lsli.q r30, r28, 2
    st.l r29, r2[r30]
    st.l r29, r2[r31]

bubble_sort_%24:
    cmp r28, r32
    bl bubble_sort_%15
    nop

bubble_sort_%27:
    addi.q r26, r26, 1
    cmp r26, r33
    bne bubble_sort_%6
    nop

bubble_sort_%30:
    ret
    nop


make_data:
make_data_%5:
    sti.b r26, r2[0]
    sti.l r28, r2[4]
    sti.l r27, r2[8]
    sti.q r6, r2[16]
    ; zext.l r26, r26
    cmp r26, r28
    bg make_data_%15
    nop

make_data_%8:
    ; zext.q r27, r27
    cmp r27, r6
    bl make_data_%17
    nop

make_data_%11:
    ; zext.q r26, r26
    add.q r26, r26, r6
    ; trunc.l r26, r26
    bra make_data_%17
    sti.l r26, r2[8]

make_data_%15:
    ; trunc.b r28, r28
    sti.b r28, r2[0]

make_data_%17:
    ret
    nop


use_data:
use_data_%1:
    ldi.b r26, r2[0]
    ; zext.q r26, r26
    ldi.q r27, r2[16]
    mul.q r27, r27, r26
    ldi.l r26, r2[8]
    ; zext.q r26, r26
    ret
    add.q r2, r27, r26


external_func_user:
external_func_user_%2:
    cmp r2, r2
    beq external_func_user_%14
    nop

external_func_user_%4:
    ldi.l r18446744073709551615, r10[0]
    <unimplemented intrinsic>
    ldi.l r26, r10[0]
    mul.l r26, r26, r27
    sti.l r26, r10[0]
    lsli.l r18446744073709551615, r26, 1
    <unimplemented intrinsic>
    sti.l r26, r10[0]
    addi.q r10, r10, 4
    cmp r10, r2
    bne external_func_user_%4
    nop

external_func_user_%14:
    ret
    nop



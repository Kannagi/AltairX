jump main ;This is the bootstrap
nop

.data
global_str : "Hello world!\x00"
base64_table : "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00>\x00\x00\x00?456789:;<=\x00\x00\x00\x00\x00\x00\x00\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x00\x00\x00\x00\x00\x00\x1a\x1b\x1c\x1d\x1e\x1f !"#$%&'()*+,-./0123\x00\x00\x00\x00\x00"

.text
get_message:
get_message_%0:
    moveu a0, global_str&0xFFFF
    ret
    smove.w a0, (global_str&0xFFFF0000)>>16


set_global:
set_global_%1:
    moveu t0, global_int&0xFFFF
    smove.w t0, (global_int&0xFFFF0000)>>16
    ret
    sti.l a0, t0[0]


parse_base64:
parse_base64_%3:
    cmpiu a1, 0
    moveu t0, 0
    moveu t1, base64_table&0xFFFF
    beq parse_base64_%47
    smove.w t1, (base64_table&0xFFFF0000)>>16

parse_base64_%8:
    ld.b n14, a0[t0]
    ori.q n13, t0, 1
    ori.q n12, t0, 2
    ori.q n11, t0, 3
    andi.b t2, n14, 127
    ld.b n13, a0[n13]
    ld.b n12, a0[n12]
    addi.q t0, t0, 4
    ld.b n11, a0[n11]
    ld.b t2, t1[t2]
    andi.b t3, n13, 127
    andi.b t4, n12, 127
    cmp t0, a1
    andi.b t5, n11, 127
    lsli.l n12, t2, 18
    ld.b t2, t1[t3]
    ld.b t3, t1[t4]
    ld.b n14, t1[t5]
    lsli.l n11, t2, 12
    lsli.l t2, t3, 6
    or.l n13, n11, n12
    or.l n12, t2, n11
    or.b n11, n14, t2
    lsri.l t2, n13, 16
    lsri.l t3, n12, 8
    sti.b n11, n15[2]
    sti.b t2, n15[0]
    sti.b t3, n15[1]
    bl parse_base64_%8
    addi.q n11, n15, 3

parse_base64_%47:
    ret
    nop


testmove:
testmove_%1:
    moveu t0, 4374
    smove.w t0, 44200
    smove.l t0, 42949
    ret
    add.q a0, a0, t0


cmove:
cmove_%2:
    ret
    call llvm.smin.i32


bar:
bar_%4:
    cmp a0, a3
    bgs bar_%12
    nop

bar_%6:
    add.l n12, n12, n11
    lsli.l n11, n12, 2
    cmp n11, a3
    bls bar_%6
    nop

bar_%12:
    ret
    add.l a0, n11, n12


foo:
foo_%2:
    moveu n11, 1
    subi.q n13, a1, 1
    moveu t0, 1
    sti.l n11, a0[0]
    cmpiu n13, 1
    bg foo_%10
    nop

foo_%7:
    lsli.q n12, n13, 2
    moveu n11, 2
    bra foo_%21
    st.l n11, a0[n12]

foo_%10:
    lsli.q n12, t0, 2
    addi.q t0, t0, 1
    ld.l n11, a0[n12]
    cmp t0, n13
    lsli.l n11, n11, 1
    bne foo_%10
    st.l n11, a0[n12]

foo_%17:
    lsli.q n12, n13, 2
    moveu n11, 2
    cmpiu a1, 0
    beq foo_%30
    st.l n11, a0[n12]

foo_%21:
    moveu t0, 0

foo_%23:
    lsli.q n12, t0, 2
    addi.q t0, t0, 1
    ld.l n11, a0[n12]
    cmp t0, a1
    addi.l n11, n11, 1
    bne foo_%23
    st.l n11, a0[n12]

foo_%30:
    ret
    nop


testptr:
testptr_%2:
    addi.q n12, a0, 16
    moveu n11, 808
    mul.q n11, a1, n11
    add.q n11, n12, n11
    ret
    addi.q a0, n11, 456


negate:
negate_%1:
    moveu n11, 0
    ret
    sub.l a0, n11, a0


sext:
sext_%1:
    ret
    sext.l a0, a0


bubble_sort:
bubble_sort_%2:
    subi.q n14, a1, 1
    moveu t0, 0
    cmpiu n14, 0
    beq bubble_sort_%32
    nop

bubble_sort_%6:
    moven n11, -1
    xor.q n11, t0, n11
    add.q n13, n11, a1
    cmpiu n13, 0
    beq bubble_sort_%29
    nop

bubble_sort_%12:
    ldi.l t1, a0[0]
    moveu t2, 0

bubble_sort_%15:
    addi.q t2, t2, 1
    lsli.q n12, t2, 2
    ld.l t3, a0[n12]
    cmp t1, t3
    bls bubble_sort_%26
    nop

bubble_sort_%22:
    lsli.q n11, t2, 2
    st.l t1, a0[n12]
    st.l t3, a0[n11]

bubble_sort_%24:
    addi.l t3, t1, 0 ; copy

bubble_sort_%26:
    cmp t2, n13
    bl bubble_sort_%15
    nop

bubble_sort_%29:
    addi.q t0, t0, 1
    cmp t0, n14
    bne bubble_sort_%6
    nop

bubble_sort_%32:
    ret
    nop


make_data:
make_data_%5:
    sti.b a1, a0[0]
    sti.l a2, a0[4]
    sti.l a3, a0[8]
    sti.q a4, a0[16]
    cmp n11, a2
    bg make_data_%15
    nop

make_data_%8:
    cmp n11, a4
    bl make_data_%17
    nop

make_data_%11:
    add.q t0, n11, a4
    bra make_data_%17
    sti.l t0, a0[8]

make_data_%15:
    sti.b n11, a0[0]

make_data_%17:
    ret
    nop


use_data:
use_data_%1:
    ldi.b t0, a0[0]
    ldi.q n11, a0[16]
    ldi.l t1, a0[8]
    mul.q n11, n11, t0
    ret
    add.q a0, n11, t1


external_func_user:
external_func_user_%2:
    subi.q sp, sp, 8
    move n11, LR
    sps.q n11, 0
    addi.q s0, a0, 0 ; copy
    addi.q s1, a1, 0 ; copy
    cmp s0, s1
    beq external_func_user_%21
    nop

external_func_user_%9:
    ldi.l a0, s0[0]
    call external_func
    ldi.l n11, s0[0]
    mul.l n11, n11, a0
    sti.l n11, s0[0]
    lsli.l a0, n11, 1
    call external_func
    sti.l a0, s0[0]
    addi.q s0, s0, 4
    cmp s0, s1
    bne external_func_user_%9
    nop

external_func_user_%19:
    addi.q s0, s1, 0 ; copy

external_func_user_%21:
    lds.q n11, 0
    move LR, n11
    addi.q sp, sp, 8
    ret
    addi.q a0, s0, 0 ; copy


use_arg_at_other_pos:
use_arg_at_other_pos_%2:
    subi.q sp, sp, 8
    move n11, LR
    sps.q n11, 0
    addi.l a0, a1, 0 ; copy
    call external_func
    lds.q n11, 0
    move LR, n11
    ret
    addi.q sp, sp, 8


lots_of_args:
lots_of_args_%10:
    subi.q sp, sp, 8
    move n11, LR
    sps.q n11, 0
    addi.q s4, a1, 0 ; copy
    cmpiu s4, 0
    beq lots_of_args_%35
    moveu a0, 0

lots_of_args_%17:
    add.l n11, a3, a2
    add.l n11, n11, a4
    sub.l s3, n11, a5
    add.l s2, a6, r18446744073709551615
    add.l s1, a7, r18446744073709551615
    moveu s0, 0
    moveu t0, 0

lots_of_args_%25:
    add.l a0, s3, t0
    call external_func
    add.l n11, s2, a0
    sub.l n11, s1, n11
    add.l a0, n11, a0
    addi.q s0, s0, 1
    cmp s0, s4
    bne lots_of_args_%25
    nop

lots_of_args_%35:
    lds.q n11, 0
    move LR, n11
    ret
    addi.q sp, sp, 8


loading:
loading_%2:
    cmpiu a1, 0
    beq loading_%12
    moveu t0, 0

loading_%5:
    lsli.q n12, t0, 2
    addi.q t0, t0, 1
    ld.l n11, a0[n12]
    cmp t0, a1
    addi.l n11, n11, 5
    bne loading_%5
    st.l n11, a0[n12]

loading_%12:
    ret
    nop


return_non_first_arg:
return_non_first_arg_%2:
    ret
    addi.l a0, a1, 0 ; copy


caller:
caller_%1:
    subi.q sp, sp, 8
    move n11, LR
    sps.q n11, 0
    call callee
    addi.l a0, a0, 5
    lds.q n11, 0
    move LR, n11
    ret
    addi.q sp, sp, 8


main:
main_%0:
    ret
    moveu a0, 25



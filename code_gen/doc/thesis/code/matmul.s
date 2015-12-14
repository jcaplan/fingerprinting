00001780 <matmul>:
    1780:	22008004 	addi	r8,r4,512
    1784:	03400074 	movhi	r13,1
    1788:	6b408004 	addi	r13,r13,512
    178c:	235b883a 	add	r13,r4,r13
    1790:	310dc83a 	sub	r6,r6,r4
    1794:	33ff8004 	addi	r15,r6,-512
    1798:	001d883a 	mov	r14,zero
    179c:	02c02004 	movi	r11,128
    17a0:	00001406 	br	17f4 <matmul+0x74>
    17a4:	5009883a 	mov	r4,r10
    17a8:	50000015 	stw	zero,0(r10)
    17ac:	6005883a 	mov	r2,r12
    17b0:	4a47883a 	add	r3,r9,r9
    17b4:	18c7883a 	add	r3,r3,r3
    17b8:	28c7883a 	add	r3,r5,r3
    17bc:	11c00017 	ldw	r7,0(r2)
    17c0:	19800017 	ldw	r6,0(r3)
    17c4:	398d383a 	mul	r6,r7,r6
    17c8:	21c00017 	ldw	r7,0(r4)
    17cc:	398d883a 	add	r6,r7,r6
    17d0:	21800015 	stw	r6,0(r4)
    17d4:	10800104 	addi	r2,r2,4
    17d8:	18c08004 	addi	r3,r3,512
    17dc:	123ff71e 	bne	r2,r8,17bc <matmul+0x3c>
    17e0:	4a400044 	addi	r9,r9,1
    17e4:	52800104 	addi	r10,r10,4
    17e8:	4affee1e 	bne	r9,r11,17a4 <matmul+0x24>
    17ec:	42008004 	addi	r8,r8,512
    17f0:	43400426 	beq	r8,r13,1804 <matmul+0x84>
    17f4:	7a15883a 	add	r10,r15,r8
    17f8:	7013883a 	mov	r9,r14
    17fc:	433f8004 	addi	r12,r8,-512
    1800:	003fe806 	br	17a4 <matmul+0x24>
    1804:	f800283a 	ret

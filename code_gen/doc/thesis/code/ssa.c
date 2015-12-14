000012c0 <g11>:
    12c0:	00800284 	movi	r2,10
    12c4:	11000816 	blt	r2,r4,12e8 <g11+0x28>
    12c8:	00000506 	br	12e0 <g11+0x20>
    12cc:	10800044 	addi	r2,r2,1
    12d0:	19400015 	stw	r5,0(r3)
    12d4:	18c00104 	addi	r3,r3,4
    12d8:	20bffc0e 	bge	r4,r2,12cc <g11+0xc>
    12dc:	f800283a 	ret
    12e0:	0005883a 	mov	r2,zero
    12e4:	00000106 	br	12ec <g11+0x2c>
    12e8:	00801904 	movi	r2,100
    12ec:	10c00044 	addi	r3,r2,1
    12f0:	18c7883a 	add	r3,r3,r3
    12f4:	18c7883a 	add	r3,r3,r3
    12f8:	01000034 	movhi	r4,0
    12fc:	21090204 	addi	r4,r4,9224
    1300:	1907883a 	add	r3,r3,r4
    1304:	014000c4 	movi	r5,3
    1308:	01001904 	movi	r4,100
    130c:	003fef06 	br	12cc <g11+0xc>

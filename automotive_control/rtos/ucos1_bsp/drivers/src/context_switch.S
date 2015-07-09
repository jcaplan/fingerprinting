	.text
	.globl context_switch

context_switch:
	 	stw   r16,  0(r4)
	 	stw   r17,  4(r4)
	 	stw   r18,  8(r4)
	 	stw   r19,  12(r4)
	 	stw   r20,  16(r4)
	 	stw   r21,  20(r4)
	 	stw   r22,  24(r4)
	 	stw   r23,  28(r4)

      	movi r16, 0
		movi r17, 0
		movi r18, 0
		movi r19, 0
		movi r20, 0
		movi r21, 0
		movi r22, 0
		movi r23, 0

		ret

	.globl context_restore

context_restore:
	 	ldw   r16,  0(r4)
        ldw   r17,  4(r4)
        ldw   r18,  8(r4)
        ldw   r19,  12(r4)
        ldw   r20,  16(r4)
        ldw   r21,  20(r4)
        ldw   r22,  24(r4)
        ldw   r23,  28(r4)

		ret

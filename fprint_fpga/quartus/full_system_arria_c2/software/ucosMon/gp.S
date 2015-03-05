	.text
	.globl get_gp

get_gp:
    	movhi r2, %hi(_gp)
    	ori r2, r2, %lo(_gp)
		ret
	.globl set_gp
set_gp:
	   	movhi r2,  0x0300
	   	ldw gp, 0(r2)
		ret
	.globl restore_gp
restore_gp:
		movhi gp, %hi(_gp)
		ori gp, gp, %lo(_gp)
		ret


;/* REGISTERS: */
;//  r0-r3 - Reserved
#define x   r4  // Column index
#define th  r5  // Threshold
#define tg  r6  // Green toggle
#define wr  r7  // Write index of process buffer
#define rd  r8  // Read index of process buffer
#define rb  r9  // Read index of pclk buffer
#define rx  r10 // Read col index
#define ry  r11 // Read row index
#define qs  r12 // Quadrant selection

;/* System Constants */
#define W                   6;//640
#define H                   3;//400
#define	WxH					9;//256000
#define Y_DEL               0x00aa
#define Y_DEL_DOUBLE        0xaaaa
#define MAX_COVERAGE        0.01; // %
#define C_FRAME_SIZE        5 ;//2560
#define IS_GREEN            0x01
#define	PCLK_REQUEST		0x0A

    area    rho, code, readonly
	preserve8
		
	extern	CAPTURE_BUFFER	
	extern	THRESH_BUFFER
	extern	DENSITY_X
	extern	DENSITY_Y					
	extern	QUADRANT_BUFFER														
	extern	QUADRANT_TOTAL											
	extern	QUADRANT_PREVIOUS												
	extern	CENTROID_X
	extern	CENTROID_Y
	extern	DENSITY_Y_MAX
	extern	CAPTURE_BUFFER_END
	extern 	CAPTURE_BUFFER_MAX
	extern	THRESH_BUFFER_END	
	extern 	THRESH_BUFFER_MAX
	extern	THRESH_VALUE		
	extern	RHO_MEM_END
	extern	CAMERA_PORT
		
	extern 	proc_flag
    entry

;/* RAM types - Pre-assigned */
;CAMERA_PORT equ 0x0000000a 				;/* GPIO_PORTA_L */
;sram		equ 0x20000000
;Dx			equ sram
;Dy			equ Dx  + W
;Q			equ Dy  + H
;QT			equ Q   + 16
;QP_ADDR		equ QT  + 4
;Ce  		equ QP_ADDR + 4 			;/* Capture end */
;Cf			equ Ce  + 4 				;/* Capture frame */
;Cm 			equ Cf  + C_FRAME_SIZE 		;/* Capture max */
;Cx			equ	Cm
;Cy			equ	Dy  + 4
;THRESH_ADDR equ Cy	+ 4
;Ym			equ THRESH_ADDR + 4 

            align
asm_test    proc
            export asm_test
            ldr wr, =THRESH_BUFFER
			ldr rd, =THRESH_BUFFER
			mov r2, #Y_DEL
add_row		strh r2, [wr], #2
			mov r1, #1
			ldr r0, =THRESH_BUFFER_MAX  ;/* if( rd < *C_FRAME_END ) */
			ldr r0, [r0]
			
fill_buff	strh r1, [wr], #2
			add r1, r1, #1
			tst r1, #1
			bne nonl
			strh r2, [wr], #2
nonl		cmp wr, r0
			blt	fill_buff
			b	rho_process
            bx  lr
            endp

			align
rho_init	proc
			export rho_init
			bx	lr
			endp
				
			align
frame_start	proc
			import 	zero_memory
			export frame_start
			stmdb  sp!, {lr}
			bl	zero_memory
			ldr wr, =THRESH_BUFFER                 ;/* wr = Cf; */
			ldr rd, =THRESH_BUFFER                 ;/* rd = Cf; */
			ldr r0, =THRESH_VALUE       	;/* th = THRESH_ADDR; */
			ldr	th, [r0]
			mov tg, #0                  ;/* Reset operation variables */
			ldr rb, =CAPTURE_BUFFER
			add rb, rb, #1
			ldmia  sp!, {lr}
			ldr r0, =proc_flag
			mov	r1, #1
			str r1, [r0]
			bx  lr
			endp

			align
frame_end	proc
			export frame_end
			ldr r0, =THRESH_BUFFER_END
			str wr, [r0]
			ldr r0, =QUADRANT_BUFFER                	;/* int * Qp = &Q; */
			ldr r2, [r0], #4            ;/* QT += *(Qp++); x4 */
			ldr r1, [r0], #4
			add r2, r2, r1
			ldr r1, [r0], #4
			add r2, r2, r1
			ldr r1, [r0]
			add r2, r2, r1
			ldr r0, =QUADRANT_TOTAL
			str r2, [r0]
			ldr r0, =proc_flag
			mov	r1, #0
			str r1, [r0]
			bx	lr
			endp

			align
row_int		proc
			export row_int
			ldr r0, =CAPTURE_BUFFER_END
			ldr r1, [r0]
			add r1, r1, #W
			str r1, [r0]
            mov  r0, #Y_DEL_DOUBLE      ;/* Load double Y_DEL to cover both RG & GB rows */
            strh r0, [wr], #2           ;/* (*(wr) = Y_DEL)++; */
			tst	tg, #1					;/* if tg is odd */
			bne	rg_row
			add rb, rb, #2				;/* GB row */
			ldr r0, =CAPTURE_BUFFER_MAX
			ldr r0, [r0]
            cmp rb, r0                ;/* If all pixels are processed go to density processor*/
            bge  rho_process
			b	x_reset
rg_row		sub rb, rb, #2				;/* RG row */
x_reset		mov x, #W           	    ;/* x = W; */
			eor tg, tg, #1    			;/* tg ^= 1; */
			bx	lr
			endp

			align
pclk_int	proc
			export pclk_int
			ldr r0, =CAPTURE_BUFFER_END
			ldr r0, [r0]
			ldr r1, =THRESH_VALUE       	;/* th = THRESH_ADDR; */
			ldr	th, [r1]
			
pclk_start  ldrb r3, [rb], #1       		;/* current_pixel = next in buffer */
			cmp r3, th					;/* if( current_pixel > th ) */
			strgeh rb, [wr], #2   		;/* (*(wr) = x)++; */
			
			cmp rb, r0
            blt pclk_start
			bx	lr
			
			endp
				
			align
rho_process proc
			import printBuffers
			export rho_process
			mov rx, #0					;/* int rx = 0, ry = 0; */
			mov ry, #0
read_loop	ldrh rx, [rd], #2         	;/* rx = *(rd++); */
			mov r0, #Y_DEL_DOUBLE
			cmp rx, r0           		;/* if( rx == Y_DEL ) */
			bne no_del
			ldr	r0, =CENTROID_Y
			ldr r0, [r0]
			cmp ry, r0              	;/* if( ry < Cy ) */
			bge higher
			and qs, qs, #0xfd  			;/* QS &= 0xfd; */
			ldr r0, =QUADRANT_BUFFER    ;/* QN = *(&Q) + *(&Q+1); */
			ldr r1, [r0], #4
			ldr r2, [r0]
			add r0, r1, r2
			b dx_store
higher		orr qs, qs, #0x02	    	;/* QS |= 0x02; */
			ldr r0, =QUADRANT_BUFFER    ;/* QN = *(&Q+2) + *(&Q+3); */
			add r0, r0, #8
			ldr r1, [r0], #4
			ldr r2, [r0]
			add r0, r1, r2
dx_store   	ldr	r3,	=QUADRANT_PREVIOUS
			ldr r1, [r3]				;/* (*(&Dx+ry++)) = QN - QP; */
			sub r1, r0, r1
			ldr	r2, =DENSITY_X
			str r1, [r2, ry]
			ldr r2, =DENSITY_Y_MAX		;/* if r1 > y_max, set as y_max */
			ldr r2, [r2]
			cmp r1, r2					
			blt	no_max
			str r1, [r2]
no_max		add ry, ry, #4
			str r0, [r3]           		;/* QP = QN; */
			b	read_loop ;inc_qs
no_del      ldr r0, =CAPTURE_BUFFER		; Load capture buffer to correct offset
			mov	r1, #0x0000ffff
			and r0, r0, r1
			sub rx, rx, r0
			ldr	r0, =CENTROID_X
			ldr r0, [r0]
			cmp rx, r0              	;/* if( rx < Cx ) */
			bge right
			and qs, qs, #0xfe	    	;/* QS &= 0xfe; */
			b 	dy_store 
right		orr qs, qs, #0x01	    	;/* QS |= 0x01; */
dy_store	ldr	r1, =DENSITY_Y
			add r2, rx, rx
			add r2, r2, r2
			ldr r0, [r1, r2]        	;/* (*(&Dy+rx))++; */
			add r0, r0, #1
			str r0, [r1, r2]
inc_qs		ldr r1, =QUADRANT_BUFFER
			add r2, qs, qs				; Multiply qs by word length
			add r2, r2, r2
			ldr r0, [r1, r2]         	;/* (*(&Q+QS))++; */
			add r0, r0, #1
			str r0, [r1, r2]
			ldr r0, =THRESH_BUFFER_END  ;/* if( rd < *C_FRAME_END ) */
			ldr r0, [r0]
			cmp rd, r0
			blt read_loop
			stmdb  sp!, {lr}
			bl 	frame_end
			ldmia  sp!, {lr}
			stmdb  sp!, {lr}
			bl 	printBuffers
			ldmia  sp!, {lr}
			bx	lr
			endp
    end

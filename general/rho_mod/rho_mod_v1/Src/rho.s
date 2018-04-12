;/* REGISTERS: */
;//  r0-r3 - Reserved
#define x   r4  // Column index
#define th  r5  // Threshold
#define tg  r6  // Green toggle
#define wr  r7  // Write index
#define rd  r8  // Read index
#define cp  r9  // Camera Port
#define rx  r10 // Read col index
#define ry  r11 // Read row index
#define QS  r12 // Quadrant selection

;/* System Constants */
#define W                   640
#define H                   400
#define Y_DEL               0xaa
#define MAX_COVERAGE        0.01 // %
#define C_FRAME_SIZE        2560
#define IS_GREEN            0x01
#define	PCLK_REQUEST		0x0A

    area    rho, code, readonly
    entry

;/* RAM types - Pre-assigned */
CAMERA_PORT equ 0x0000000a 				;/* GPIO_PORTA_L */
sram		equ 0x20000000
Dx			equ sram
Dy			equ Dx  + W
Q			equ Dy  + H
QT			equ Q   + 16
QP_ADDR		equ QT  + 4
Ce  		equ QP_ADDR + 4 			;/* Capture end */
Cf			equ Ce  + 4 				;/* Capture frame */
Cm 			equ Cf  + C_FRAME_SIZE 		;/* Capture max */
Cx			equ	Cm
Cy			equ	Dy  + 4
THRESH_ADDR equ Cy	+ 4

			align
rho_init	proc
			export rho_init
			ldr cp, =CAMERA_PORT
			bx	lr
			endp
				
			align
frame_start	proc
			export frame_start

			ldr r0, =Dx                 ;/* Clear RAM */
			mov r1, #0
			ldr r2, =Cm
zero		str	r1, [r0], #4
			cmp	r0, r2
			blt zero
			ldr wr, =Cf                 ;/* wr = Cf; */
			ldr rd, =Cf                 ;/* rd = Cf; */
			ldr th, =THRESH_ADDR       	;/* th = THRESH_ADDR; */
			mov tg, #0                  ;/* Reset operation variables */
			b   rho_process             ;/* Start processing loop */
			bx  lr
			endp

			align
asm_test	proc
			export asm_test
			ldr r0, =Q
			mov	r1, #1
			str r1, [r0], #4
			add r1, #1
			str r1, [r0], #4
			add r1, #1
			str r1, [r0], #4
			add r1, #1
			str r1, [r0]
			bx	lr
			endp

			align
frame_end	proc
			export frame_end
			ldr r0, =Ce
			str wr, [r0]
			ldr r0, =Q                	;/* int * Qp = &Q; */
			ldr r2, [r0], #4            ;/* QT += *(Qp++); x4 */
			ldr r1, [r0], #4
			add r2, r2, r1
			ldr r1, [r0], #4
			add r2, r2, r1
			ldr r1, [r0]
			add r2, r2, r1
			ldr r0, =QT
			str r2, [r0]
			bx	lr
			endp

			align
row_int		proc
			export row_int
			mov r0, #Y_DEL
			str r0, [wr]     			;/* (*(wr) = Y_DEL)++; */
			add wr, wr, #1
			eor tg, tg, #0x01    		;/* tg ^= 0x01; */
			mov x, tg           		;/* x = tg; */
			bx	lr
			endp

			align
pclk_int	proc
			export pclk_int
			add x, x, #4       		 	;/* x++ */
			swi PCLK_REQUEST
			bx lr
			endp

			align
pclk_swi	proc
			export pclk_swi
			tst x, #0x01         		;/* if( x & 0x01 ) burn */
			beq burn_pixel
			ldr r3, [cp]        		;/* if( cp > th ) */
			cmp r3, th
			strge x, [wr], #1   		;/* (*(wr) = x)++; */
burn_pixel  bx lr
			endp
				
			align
rho_process proc
			export rho_process
			mov rx, #0					;/* int rx = 0, ry = 0; */
			mov ry, #0
read_loop	cmp rd, wr             	 	;/* if( rd < wr ) */
			bcs read_loop
			ldr rx, [rd], #1         	;/* rx = *(rd++); */
			mov r0, #Y_DEL
			cmp rx, r0           		;/* if( rx == Y_DEL ) */
			bne no_del
			ldr	r0, =Cy
			cmp ry, r0              	;/* if( ry < Cy ) */
			bge higher
			and QS, QS, #0xfd  			;/* QS &= 0xfd; */
			ldr r0, =Q               	;/* QN = *(&Q) + *(&Q+1); */
			ldr r1, [r0], #4
			ldr r2, [r0]
			add r0, r1, r2
			b dx_store
higher		orr QS, QS, #0x02	    	;/* QS |= 0x02; */
			ldr r0, =Q               	;/* QN = *(&Q+2) + *(&Q+3); */
			add r0, r0, #8
			ldr r1, [r0], #4
			ldr r2, [r0]
			add r0, r1, r2
dx_store   	ldr	r3,	=QP_ADDR
			ldr r1, [r3]				;/* (*(&Dx+ry++)) = QN - QP; */
			sub r1, r0, r1
			ldr	r2, =Dx
			str r1, [r2, ry]
			add ry, ry, #4
			str r0, [r3]           		;/* QP = QN; */
no_del      ldr	r0, =Cx
			cmp rx, r0              	;/* if( rx < Cx ) */
			bge right
			and QS, QS, #0xfe	    	;/* QS &= 0xfe; */
			b 	dy_store 
right		orr QS, QS, #0x01	    	;/* QS |= 0x01; */
dy_store	ldr	r1, =Dy
			ldr r0, [r1, rx]        	;/* (*(&Dy+rx))++; */
			add r0, r0, #1
			str r0, [r1, rx]
			ldr r1, =Q
			ldr r0, [r1, QS]         	;/* (*(&Q+QS))++; */
			add r0, r0, #1
			str r0, [r1, QS]

			ldr r0, =Ce   				;/* if( rd < *C_FRAME_END ) */
			ldr r0, [r0]
			cmp rd, r0
			blt read_loop
			bx	lr
			endp
    end

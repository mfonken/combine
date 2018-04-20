;/* REGISTERS: */
;//  r0-r3 - Reserved
#define tx  r4  // Column index
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

;//#define BAYER_TOGGLE

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
            ldmia  sp!, {lr}
			mov tg, #0                  ;/* Reset operation variables */
			ldr rb, =CAPTURE_BUFFER
			;add rb, rb, #1
            ldr wr, =THRESH_BUFFER      ;/* wr = Cf; */
            ldr rd, =THRESH_BUFFER      ;/* rd = Cf; */
            ldr r0, =THRESH_VALUE       ;/* th = THRESH_ADDR; */
            ldr th, [r0]

; Set process flag
			ldr r0, =proc_flag
			mov	r1, #1
			str r1, [r0]

			bx  lr
			endp

			align
frame_end	proc
			export frame_end
; Add the four quadrants into QUADRANT_TOTAL
			ldr r0, =QUADRANT_BUFFER    ;/* int * Qp = &Q; */
			ldr r2, [r0], #4            ;/* QT += *(Qp++); x4 */
			ldr r1, [r0], #4
			add r2, r2, r1
			ldr r1, [r0], #4
			add r2, r2, r1
			ldr r1, [r0]
			add r2, r2, r1
			ldr r0, =QUADRANT_TOTAL
			str r2, [r0]
; Reset process flag
			ldr r0, =proc_flag
			mov	r1, #0
			str r1, [r0]
			bx	lr
			endp

			align
row_int		proc
			export row_int

; Increment End of capture buffer
			ldr r1, =CAPTURE_BUFFER_END
			ldr r0, [r1]
			add r0, r0, #W
			str r0, [r1]
; Store Y delimiter in thresh buffer
            mov  r0, #Y_DEL_DOUBLE      ;/* Load double Y_DEL to cover both RG & GB rows */
            strh r0, [wr], #2           ;/* (*(wr) = Y_DEL)++; */
#ifdef BAYER_TOGGLE
			tst	tg, #1					;/* if tg is odd */
			bne	rg_row
			add rb, rb, #2				;/* GB row */
            b   check_rmax
rg_row		sub rb, rb, #2				;/* RG row */
#endif
check_rmax  ldr r0, =CAPTURE_BUFFER_MAX
            ldr r0, [r0]
            cmp rb, r0                  ;/* If all pixels are processed go to density processor */
            ble not_done
			
			ldr r0, =THRESH_BUFFER_END	; Set final end of thresh buffer for frame
			str wr, [r0]
			b	rho_process
			
not_done	nop
#ifdef BAYER_TOGGLE
		    eor tg, tg, #1    			;/* Otherwise toggle tg register */
#endif
			bx	lr
			endp

			align
pclk_int	proc
			export pclk_int
			ldr r1, =THRESH_VALUE       	;/* th = THRESH_ADDR; */
			ldr	th, [r1]
			
pclk_start  ldrb r3, [rb], #1       		;/* current_pixel = next in buffer */
			cmp r3, th					;/* if( current_pixel > th ) */
			strgeh rb, [wr], #2   		;/* (*(wr) = x)++; */
			
			ldr r1, =CAPTURE_BUFFER_END
			ldr r0, [r1]
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
			cmp rx, r0       ;/* if( rx == Y_DEL ) */
			bne no_del

; Process Row
            ldr r0, =QUADRANT_BUFFER
			ldr	r1, =CENTROID_Y
			ldrh r1, [r1]
			cmp ry, r1              	;/* if( ry < Cy ) */
			bge higher
			and qs, qs, #0xfd  			;/* QS &= 0xfd; */
			ldr r1, [r0], #4            ;/* QN = *(&Q) + *(&Q+1); */
			ldr r2, [r0]
			add r0, r1, r2
			b dx_store
higher		orr qs, qs, #0x02	    	;/* QS |= 0x02; */
			add r0, r0, #8              ;/* QN = *(&Q+2) + *(&Q+3); */
			ldr r1, [r0], #4
			ldr r2, [r0]
            add r0, r1, r2              ; Calculate QN into r0
dx_store   	ldr	r1,	=QUADRANT_PREVIOUS
			ldr r2, [r1]				;/* (*(&Dx+ry++)) = QN - QP; */
            str r0, [r1]                ;/* QP = QN; */
            sub r0, r0, r2              ; Calculate QN - QP into r0
			ldr	r2, =DENSITY_X
			str r0, [r2, ry]
            add ry, ry, #4
			ldr r3, =DENSITY_Y_MAX		;/* if r1 > y_max, set as y_max */
			ldr r2, [r3]
			ldr r1, [r1]
			cmp r1, r2					
            ble	rho_lcheck              ; Contiunue (to quadrant increment) if less than or equal
            str r1, [r3]                ; Store as new max if greater
            b	rho_lcheck

; Process Pixel
no_del  	ldr r0, =CAPTURE_BUFFER		; Load capture buffer to correct offset
            mov	r1, #0x0000ffff
            and r0, r0, r1              ; Keep bottom half in r0
			cmp	rx, r0
			blt	rho_lcheck				; Check for valid value
            sub rx, rx, r0              ; Remove address offset from rx
			sub rx, rx, #1
			mov	r0, #W
sudo_mod	cmp rx, r0
			blt	rx_corr
			sub rx, rx, r0
			b	sudo_mod
rx_corr		ldr	r1, =CENTROID_X
			ldrh r1, [r1]
			cmp rx, r1              	;/* if( rx < Cx ) */
			bge right
left		and qs, qs, #0xfe	    	;/* QS &= 0xfe; */
			b 	dy_store 
right		orr qs, qs, #0x01	    	;/* QS |= 0x01; */
dy_store	ldr	r1, =DENSITY_Y
            add r2, rx, rx              ; Calcalute rx x4 into r2
            add r2, r2, r2              ; (rx is byte width while Dx is word width)
			ldr r0, [r1, r2]        	;/* (*(&Dy+rx))++; */
			add r0, r0, #1
			str r0, [r1, r2]

; Increment Quandrants
inc_qs		ldr r1, =QUADRANT_BUFFER
			add r2, qs, qs				; Multiply qs by word length
			add r2, r2, r2
			ldr r0, [r1, r2]         	;/* (*(&Q+QS))++; */
			add r0, r0, #1
			str r0, [r1, r2]

; Loop until Thresh Buffer End
rho_lcheck	ldr r0, =THRESH_BUFFER_END  ;/* if( rd < *C_FRAME_END ) */
			ldr r0, [r0]
			cmp rd, r0
			blt read_loop

			stmdb  sp!, {lr}
			bl	frame_end
			mov	r0, #10
			bl 	printBuffers
			ldmia  sp!, {lr}
			bx	lr
			endp
    end

;/* REGISTERS: */
;//  r0-r3 - Reserved
#define tg  r8  // Green toggle
#define rb  r9  // Write index of process buffer
#define wr  r10  // Read index of process buffer
#define rd  r12  // Read index of pclk buffer
#define th	r11	// Threshold (Shares with rd)

#define rx  r6 // Read col index
#define ry  r7 // Read row index
#define qs  r8 // Quadrant selection

;/* System Constants */
#define HALF_WORD_WIDTH     160
#define Y_DEL_DOUBLE        0xaaaa

;#define STATIC_BUFFER
;#define BAYER_TOGGLE
;#define SPOOF


#ifdef BAYER_TOGGLE
#define PXL_JMP 2
#else
#define PXL_JMP 1
#endif

    area    rho, code, readonly
	preserve8

	;extern WR
	;extern RB

	extern	CAPTURE_BUFFER
	extern	THRESH_BUFFER
	extern	DENSITY_X
	extern	DENSITY_Y
	extern	QUADRANT_BUFFER
	extern	QUADRANT_TOTAL
	extern	QUADRANT_PREVIOUS
	extern	CENTROID_X
	extern	CENTROID_Y
	extern	DENSITY_X_MAX
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
            mov r0, #0
			mov r1, #1
			mov r2, #2
			mov r3, #3
			mov r4, #4
			mov r5, #5
			mov r6, #6
			mov r7, #7
			mov r8, #8
			mov r9, #9
			mov r10, #10
			mov r11, #11
			mov r12, #12
			;vmov.f32 s24, #24
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
			ldr rb, =CAPTURE_BUFFER
#ifdef BAYER_TOGGLE
			mov tg, #0                  ;/* Reset operation variables */
#endif
            ldr wr, =THRESH_BUFFER      ;/* wr = Cf; */
            ldr r0, =THRESH_VALUE       ;/* th = THRESH_ADDR; */
            ldr th, [r0]
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
			bx	lr
			endp

			align
row_int		proc
			export row_int

			ldr r0, =THRESH_VALUE       ;/* th = THRESH_ADDR; */
			ldr th, [r0]

			; Store Y delimiter in thresh buffer
			mov  r0, #Y_DEL_DOUBLE      ;/* Load double Y_DEL to cover both RG & GB rows */
			strh r0, [wr], #2           ;/* (*(wr) = Y_DEL)++; */
			ldr r0, =CAPTURE_BUFFER
			mov r0, rb

#ifdef BAYER_TOGGLE
			eor tg, tg, #1    			;/* Otherwise toggle tg register */
			tst	tg, #1					;/* if tg is odd */
			bne	row_ret
			add rb, rb, #1				;/* GB row */
#endif
row_ret		bx	lr
			endp

			align
pixel_proc	proc
			export pixel_proc
			
			ldr r1, =THRESH_BUFFER_MAX
			ldr r1, [r1]
			cmp wr, r1
			blt pxl_start
			mov	r0, #0
			bx	lr
			
pxl_start  	ldrb r3, [rb], #PXL_JMP    	;/* current_pixel = next in buffer */
			cmp r3, th				    ;/* if( current_pixel > th ) */
			strgeh rb, [wr], #2   		;/* (*(wr) = x)++; */
			mov	r0, #1
			bx	lr
			endp

			align
rho_proc proc
			import printBuffers
			export rho_proc
			stmdb  sp!, {r0-r12,lr}
			
#ifdef SPOOF
			ldr wr, =THRESH_BUFFER
			add wr, wr, r0
#endif
			
			ldr r1, =THRESH_BUFFER_END	; Set final end of thresh buffer for frame
			str wr, [r1]

			ldr rd, =THRESH_BUFFER
			mov rx, #0					;/* int rx = 0, ry = 0; */
			mov ry, #0
			mov qs, #0

read_loop	ldrh rx, [rd], #2         	;/* rx = *(rd++); */
			mov r0, #Y_DEL_DOUBLE
			cmp rx, r0       			;/* if( rx == Y_DEL ) */
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
			ldr r3, =DENSITY_X_MAX		;/* if r1 > x_max, set as x_max */
			ldr r2, [r3]
			cmp r0, r2
            ble	rho_lcheck              ; Contiunue (to quadrant increment) if less than or equal
            str r0, [r3]                ; Store as new max if greater
            b	rho_lcheck

; Process Pixel
no_del  	ldr r0, =CAPTURE_BUFFER		; Load capture buffer to correct offset
            mov	r1, #0x0000ffff
            and r0, r0, r1              ; Keep bottom half in r0
			cmp	rx, r0

#ifdef SPOOF
			b	rx_corr
#endif
			
			blt	rho_lcheck				; Check for valid value
            sub rx, rx, r0              ; Remove address offset from rx
			sub rx, rx, #1
			mov	r0, #HALF_WORD_WIDTH
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
			ldmia  sp!, {lr}
			ldmia  sp!, {r0-r12,lr}
			bx	lr
			endp
    end

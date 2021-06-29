format ELF64

	public task
	msg db "Начало вывода fasm",0xA, 0
	array1 dq ?
	array2 dq ?
	array3 dq ?
	brk_firstlocation dq 0
	brk_secondlocation dq 0
	brk_thirdlocation dq 0
	seed     dd  0
	minValue dd 1
	maxValue dd 10000


	; rdi = arraysize1 
	; rsi = arraysize2
	;output:
	; rax 
	task:
		push rdx r8 r9 r10 r11 rdi rsi r12 r13
		mov r12, rdi
		mov r13, rsi	

		;ВЫДЕЛЕНИЕ ПАМЯТИ
		mov rax, 12  ; sys_brk
		xor rdi, rdi ; в rdi записываем ноль( получить текущий адрес)
    	syscall
    	mov [brk_firstlocation], rax
    	; в rax у нас указатель на текущую границу кучи
		; новый адрес нужно записать в регистр rdi
		mov rcx, 0xff; количество хидеров файлов, которые надо хранить
		lea rdi, [rcx*8 + rax] ; каждый хидер занимает восемь байт
		mov rax, 12 ; sys_brk
		syscall   

		mov [brk_secondlocation], rax
    	; в rax у нас указатель на текущую границу кучи
		; новый адрес нужно записать в регистр rdi
		mov rcx, 0xff; количество хидеров файлов, которые надо хранить
		lea rdi, [rcx*8 + rax] ; каждый хидер занимает восемь байт
		mov rax, 12 ; sys_brk
		syscall   

		mov [brk_thirdlocation], rax
    	; в rax у нас указатель на текущую границу кучи
		; новый адрес нужно записать в регистр rdi
		mov rcx, 0xff; количество хидеров файлов, которые надо хранить
		lea rdi, [rcx*8 + rax] ; каждый хидер занимает восемь байт
		mov rax, 12 ; sys_brk
		syscall   
		 
		mov rax, msg
		call print_string

		mov rcx, r12 ; array1_sizei
		mov rsi, [brk_firstlocation] ;array 
		.array1_handl:
			mov rdx, r12
			sub rdx, rcx ; invert index of array
			call get_rnd 
			mov [rsi+rdx*8], rax
			mov rax, [rsi+rdx*8]
			call print_number
			mov rax, 0x20 ; пробел
			call print_char
			loop .array1_handl
		call print_line

		mov rcx, r13 ; array2_size
		mov r10, [brk_secondlocation]
		.array2_handl:
			mov rdx, r13
			sub rdx, rcx ; invert index of array
			call get_rnd 
			mov [r10+rdx*8], rax
			mov rax, [r10+rdx*8]
			call print_number
			mov rax, 0x20 ; пробел
			call print_char
			loop .array2_handl
		call print_line

		mov rcx, r12 
		mov r9, [brk_firstlocation]
		mov r10, [brk_thirdlocation] 
		.array3_handl1:
			mov rdx, r12
			sub rdx, rcx ; invert index of array
			mov rax, [r9+rdx*8] 
			mov [r10+rdx*8], rax
			loop .array3_handl1

		mov rcx, r13 
		mov r9, [brk_secondlocation]
		mov r10, [brk_thirdlocation]
		mov rbx, r12
		.array3_handl2:
			mov rdx, r13
			sub rdx, rcx ; invert index of array
			mov rax, [r9+rdx*8] 
			mov [r10+rbx*8], rax
			inc rbx
			loop .array3_handl2

   		mov rax, r10
		.close:
			pop r13 r12 rsi rdi r11 r10 r9 r8 rdx
			ret


	;input
	; minValue
	; maxValue
	;output
	; eax
	get_rnd:
		push rbx rcx rdx
		mov	eax, seed
		or eax, eax
		jnz	.zero
		.zero:
			rdtsc ; Read Time Stamp Counter 
			or	eax, eax
			jz	.notzero
		.notzero:
			xor	edx, edx
			mov	ebx, 127773
			div	ebx
			push rax 
			mov	 eax, 16807 
			mul	edx 
			pop	rdx 
			push rax 
			mov	eax, 2836 
			mul	edx 
			pop	rdx 
			sub	edx, eax 
			mov	eax, edx 
			mov	[seed], edx
			xor	edx, edx
			mov	ebx, [maxValue] 
			sub	ebx, [minValue] 
			inc	ebx
			div	ebx
			mov	eax, edx
			add	eax, [minValue]
			pop	rdx rcx rbx 
		    ret

	; input:
	; rax = number
	print_number:
		push rax rbx rcx rdx
		xor rcx, rcx
	    .next_iter:
	        cmp rax, 0
	        je .print_iter   
	        mov rbx, 10
	        xor rdx, rdx
	        div rbx ; 		525/10 = 52 -> rax , 5 -> rdx
	        add rdx, '0' ; 		nomer symvola 0 v ascii + 5 = nomer symvola 5
	        push rdx
	        inc rcx
	    	jmp .next_iter
	    .print_iter:
	    	cmp rcx, 0
	    	je .close
	    	pop rax
	    	call print_char
	    
	    	dec rcx
	    	jmp .print_iter
	    .close:
	    	pop rdx rcx rbx rax
			ret

	;input:
	;rax = char
	print_char:
		push rax
		push rbx
		push rcx
		push rdx
		push rsi
		push rax
		mov rsi, rsp
		pop rax
		xor rax, rax
		mov rdx, 1 ;
		mov rax, 1 ; Write
		mov rdi, 1 ; Destination (1 - stdout) register rsp - last value in stack 
		syscall
		pop rsi
		pop rdx
		pop rcx
		pop rbx
		pop rax
		ret 

print_line:
	push rax
	mov rax, 0xA
	call print_char
	pop rax
	ret

;input:
;rax = string
print_string:
	push rax rbx rcx rdx 
	mov rcx, rax
	call length_string ; now rax have LEngth
	mov rdx, rax
	mov rax, 4
	mov rbx, 1
	int 0x80
	pop rdx rcx rbx rax
	ret; return to call place!

;input:
;rcx = string
;output:
;rax = length	
length_string:
	push rdx
	xor rdx, rdx
	.next_iter: ; procedure from dot (.) is local procedure
		cmp [rax+rdx], byte 0 ; [] to take value from register
		je .close
		inc rdx
		jmp .next_iter
	.close:
		mov rax, rdx
		pop rdx
		ret


exit:
	mov rax, 1 ; 1 - exit()
	mov rbx, 0 ; 0 - state code
	int 0x80
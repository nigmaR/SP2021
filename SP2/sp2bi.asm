format ELF64 executable 3
entry _start

segment readable writeable
	array db 1,2,3,4,5,6,7,8,9,10
	array_size = $ - array

segment readable executable
	_start:
		mov rsi, array
		mov rcx, array_size
		call task
		mov rax, rbx
		call print_number
		call print_line
		call exit

	;input: 
	; rsi = array
	; rcx = array_size
	;output:
	; rbx 
	task:
		push rax
		push rdx
		push r8
		push r9
		xor rbx, rbx
		xor r9, r9
		mov r8b, cl ; array_size
		.array_handl:
			mov dl, r8b
			sub dl, cl ; invert index of array
			mov al, [rsi+rdx]
			call print_number
			mov al, '='
			call print_char
			test dl, 1
			jnz .odd
			jz .even
			.odd:
				sar byte [rsi+rdx], 1
				jmp .sum
			.even:
				shl byte [rsi+rdx], 1
			.sum:
				mov al, [rsi+rdx]
				call print_number
				call print_line
				mov r9b, [rsi+rdx]
				add rbx, r9
			loop .array_handl
		.close:

			pop r9
			pop r8
			pop rdx
			pop rax
			ret

	; input:
	; rax = number
	print_number:
		push rax
		push rbx
		push rcx
		push rdx
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
	    	pop rdx
	    	pop rcx
	    	pop rbx
	    	pop rax
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

exit:
	mov rax, 1 ; 1 - exit()
	mov rbx, 0 ; 0 - state code
	int 0x80
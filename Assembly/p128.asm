assume cs:codesg

codesg segment

  ;��dw����16���������ݣ��ڳ�����غ󣬽�ȡ��16���ֵ��ڴ�ռ䣬�����16�����ݡ�
  ;�ں���ĳ����н���οռ䵱��ջ��ʹ��
  dw 0123h,0456h,0789h,0abch,0defh,0fedh,0cbah,0987h  
  dw 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

  start:	mov ax,cs
		mov ss,ax
		mov sp,30h	;������ջ��ss:spָ��cs:30

		mov bx,0
		mov cx,8
	s:	push cs:[bx]
		add bx,2
		loop s		;���Ͻ������0~15��Ԫ�е�8����������������ջ

		mov bx,0
		mov cx,8
	s0:	pop cs:[bx]
		add bx,2
		loop s0

		mov ax,4c00h
		int 21h

codesg ends

end start			;ָ������������start��	
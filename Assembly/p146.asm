assume cs:codesg,ds:datasg

datasg segment
 db 'BaSic'
 db 'MiNiX'
datasg ends
	
codesg segment
 start:	mov ax,datasg
	mov ds,ax

	mov bx,0

	mov cx,5	;����ѭ������5

     s: mov al,0[bx]	;[bx+0]��λ��һ���ַ����е��ַ�
        and al,11011111B
	mov [bx],al

	mov al,5[bx]	;[bx+5]��λ��һ���ַ����е��ַ�
        or al,00100000B
	mov [bx],al

	inc bx
	loop s

	mov ax,4c00h
	int 21h

codesg ends

end start
assume cs:code
code segment 

    mov ax,0ffffh
    mov ds,ax
    mov bx,0		;��ʼ��ds:bxָ��ffff:0

    mov dx,0		;��ʼ���ۼӼĴ���dx��(dx)=0

    mov cx,12		;��ʼ��ѭ�������Ĵ���cx��(cx)=12

  s:mov al,[bx]
    mov ah,0
    add dx,ax		;�����dx��((ds)*16+(bx))��Ԫ����ֵ
    inc bx		;ds:bxָ����һ����Ԫ
    loop s

    mov ax,4c00h
    int 21h

code ends
end
;������:
;Board:HC6800-EM3 V3.0
;IC:STC12C5A16S2
;Attention:STC-ISP NEED power-down and power-on !!!

LOOP: SETB P0.0   ;(1)Ϩ���
LCALL DELAY       ;(2)��ʱһ��ʱ��
CLR P0.0          ;(3)������
LCALL DELAY       ;(4)��ʱһ��ʱ��
AJMP LOOP         ;(5)��ת����һ��LOOP��

;�����ӳ���
DELAY: MOV R7,#250;(6)
D1: MOV R6,#250   ;(7)
D2: MOV R5,#10   ;(7)
D3: DJNZ R5,D3    ;(8)
    DJNZ R6,D2
    DJNZ R7,D1    ;(9)
    RET           ;(10)
END               ;(11)
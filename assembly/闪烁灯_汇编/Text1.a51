;主程序:
;Board:HC6800-EM3 V3.0
;IC:STC12C5A16S2
;Attention:STC-ISP NEED power-down and power-on !!!

LOOP: SETB P0.0   ;(1)熄灭灯
LCALL DELAY       ;(2)延时一段时间
CLR P0.0          ;(3)点亮灯
LCALL DELAY       ;(4)延时一段时间
AJMP LOOP         ;(5)跳转到第一句LOOP处

;以下子程序
DELAY: MOV R7,#250;(6)
D1: MOV R6,#250   ;(7)
D2: MOV R5,#10   ;(7)
D3: DJNZ R5,D3    ;(8)
    DJNZ R6,D2
    DJNZ R7,D1    ;(9)
    RET           ;(10)
END               ;(11)
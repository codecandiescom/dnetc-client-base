; BDESCHG.INC

; Part of BrydDES Key Search Library version 1.01.
; Date: January 17, 1998.
; Copyright Svend Olaf Mikkelsen, 1995, 1997, 1998.


; Please read README.TXT.

; Written for Microsoft Macro Assembler, version 6.00B.
; Modified, so it also seems to assemble correct with TASM 5.0.


change1   macro
changetwokeydword    1,00400000h, 3,10000000h
changetwokeydword    6,01000000h, 7,00040000h
changetwokeydword    9,40000000h,12,00800000h
changetwokeydword   14,00000001h,17,08000000h
changetwokeydword   20,04000000h,22,00000004h
changetwokeydword   23,20000000h,25,00080000h
changetwokeydword   28,00000002h,32,00400000h
endm

change2   macro
changetwokeydword    1,20000000h, 4,00000008h
changetwokeydword    6,02000000h, 7,04000000h
changetwokeydword   10,00400000h,11,10000000h
changetwokeydword   14,01000000h,15,00040000h
changetwokeydword   18,80000000h,19,00200000h
changetwokeydword   24,08000000h,25,08000000h
changethreekeydword 28,04000000h,30,00000004h,31,00800000h
endm

change3   macro
changetwokeydword    2,08000000h, 5,00100000h
changetwokeydword    9,00800000h,12,00000008h
changetwokeydword   14,02000000h,15,04000000h
changetwokeydword   19,00400000h,22,40000000h
changetwokeydword   23,80000000h,26,80000000h
changetwokeydword   27,00200000h,32,00000001h
endm

change4   macro
changetwokeydword    2,00004000h, 6,00040000h
changetwokeydword    7,00000020h, 9,00000400h
changetwokeydword   11,00000080h,14,00000800h
changetwokeydword   15,00000040h,18,00080000h
changetwokeydword   20,00000200h,22,00010000h
changetwokeydword   25,00000800h,28,00020000h
changetwokeydword   29,00000008h,32,00000100h
endm

change5   macro
changetwokeydword    1,00002000h, 3,00000020h
changetwokeydword    5,00000400h, 7,00000080h
changetwokeydword   10,00000800h,11,00000040h
changetwokeydword   13,00004000h,18,00010000h
changetwokeydword   21,00000800h,24,00020000h
changetwokeydword   25,00000008h,28,00004000h
changetwokeydword   30,00000040h,32,00040000h
endm

change6   macro
changetwokeydword    3,00000040h, 5,00004000h
changetwokeydword    9,00000010h,12,00000080h
changetwokeydword   13,00000004h,15,00008000h
changetwokeydword   17,00000008h,20,00004000h
changetwokeydword   22,00000040h,23,00002000h
changetwokeydword   26,00000400h,28,00008000h
changetwokeydword   29,00001000h,32,00000800h
endm

change7   macro
changetwokeydword    4,00000080h, 5,00000004h
changetwokeydword    7,00008000h,10,00000100h
changetwokeydword   14,00040000h,15,00000020h
changetwokeydword   18,00000400h,20,00008000h
changetwokeydword   21,00001000h,26,00080000h
changethreekeydword 28,00000200h,30,00010000h,31,00000010h
endm

change9   macro
changetwokeydword    1,10000000h, 4,40000000h
changetwokeydword    5,80000000h, 8,80000000h
changetwokeydword    9,00200000h,14,08000000h
changetwokeydword   15,08000000h,17,00100000h
changetwokeydword   21,00800000h,24,00000008h
changetwokeydword   26,02000000h,27,04000000h
changetwokeydword   30,00400000h,31,00400000h
endm

change10   macro
changetwokeydword    2,00000008h, 3,00080000h
changetwokeydword    6,00000002h, 9,00400000h
changetwokeydword   12,40000000h,13,80000000h
changetwokeydword   16,80000000h,17,40000000h
changetwokeydword   20,00800000h,22,00000001h
changethreekeydword 25,00100000h,29,00800000h,31,20000000h
endm

change11   macro
changetwokeydword    3,08000000h, 6,04000000h
changetwokeydword    8,00000004h, 9,20000000h
changetwokeydword   11,00080000h,14,00000002h
changetwokeydword   18,00400000h,19,10000000h
changetwokeydword   22,01000000h,23,00040000h
changetwokeydword   25,40000000h,28,00800000h
changetwokeydword   30,00000001h,32,08000000h
endm

change12   macro
changetwokeydword    4,00000040h, 5,00002000h
changetwokeydword    8,00000400h,10,00008000h
changetwokeydword   11,00001000h,16,00080000h
changetwokeydword   17,00004000h,21,00000010h
changetwokeydword   24,00000080h,25,00000004h
changethreekeydword 27,00008000h,30,00000100h,32,00004000h
endm

change13   macro
changetwokeydword    1,00000020h, 4,00000400h
changetwokeydword    6,00008000h, 7,00001000h
changetwokeydword   12,00080000h,14,00000200h
changetwokeydword   16,00010000h,17,00000010h
changetwokeydword   20,00000080h,21,00000004h
changetwokeydword   23,00008000h,26,00000100h
changetwokeydword   30,00040000h,31,00002000h
endm

change14   macro
changetwokeydword    1,00000040h, 4,00080000h
changetwokeydword    6,00000200h, 8,00010000h
changetwokeydword   11,00000800h,14,00020000h
changetwokeydword   15,00000008h,18,00000100h
changetwokeydword   22,00040000h,23,00000020h
changethreekeydword 25,00000400h,27,00000080h,30,00000800h
endm

change15   macro
changetwokeydword    2,00000080h, 3,00000800h
changetwokeydword    6,00020000h, 7,00000008h
changetwokeydword   10,00004000h,12,00000040h
changetwokeydword   13,00002000h,16,00000400h
changetwokeydword   17,00000400h,19,00000080h
changetwokeydword   22,00000800h,23,00000040h
changetwokeydword   25,00004000h,29,00000010h
endm

change17   macro
changetwokeydword    2,40000000h, 4,01000000h
changetwokeydword    5,00040000h, 7,40000000h
changetwokeydword   10,00800000h,12,00000001h
changetwokeydword   15,00100000h,18,04000000h
changetwokeydword   20,00000004h,21,20000000h
changetwokeydword   23,00080000h,26,00000002h
changetwokeydword   29,00400000h,31,10000000h
endm

change18   macro
changetwokeydword    1,00080000h, 4,02000000h
changetwokeydword    5,04000000h, 8,00400000h
changetwokeydword    9,10000000h,12,01000000h
changetwokeydword   13,00040000h,15,40000000h
changetwokeydword   17,00200000h,22,08000000h
changetwokeydword   23,08000000h,26,04000000h
changethreekeydword 28,00000004h,29,20000000h,32,00000008h
endm

change19   macro
changetwokeydword    1,08000000h, 3,00100000h
changetwokeydword    7,00800000h,10,00000008h
changetwokeydword   12,02000000h,13,04000000h
changetwokeydword   16,00400000h,17,00400000h
changetwokeydword   20,40000000h,21,80000000h
changethreekeydword 24,80000000h,25,00200000h,30,08000000h
endm

change20   macro
changetwokeydword    2,00000040h, 4,00040000h
changetwokeydword    5,00000020h, 7,00000400h
changetwokeydword    9,00000080h,12,00000800h
changetwokeydword   13,00000040h,15,00004000h
changetwokeydword   18,00000200h,20,00010000h
changetwokeydword   23,00000800h,26,00020000h
changetwokeydword   27,00000008h,30,00004000h
endm

change21   macro
changetwokeydword    2,00000400h, 3,00000400h
changetwokeydword    5,00000080h, 8,00000800h
changetwokeydword    9,00000040h,11,00004000h
changetwokeydword   15,00000010h,19,00000800h
changetwokeydword   22,00020000h,23,00000008h
changetwokeydword   26,00004000h,28,00000040h
changetwokeydword   29,00002000h,31,00000020h
endm

change22   macro
changetwokeydword    2,00080000h, 3,00004000h
changetwokeydword    7,00000010h,10,00000080h
changetwokeydword   11,00000004h,13,00008000h
changetwokeydword   16,00000100h,18,00004000h
changetwokeydword   20,00000040h,21,00002000h
changetwokeydword   24,00000400h,26,00008000h
changetwokeydword   27,00001000h,31,00000040h
endm

change23   macro
changetwokeydword    1,00000800h, 3,00000004h
changetwokeydword    5,00008000h, 8,00000100h
changetwokeydword   12,00040000h,13,00000020h
changetwokeydword   15,00000400h,18,00008000h
changetwokeydword   19,00001000h,24,00080000h
changethreekeydword 26,00000200h,28,00010000h,32,00000080h
endm

change25   macro
changetwokeydword    2,01000000h, 3,80000000h
changetwokeydword    6,80000000h, 7,00200000h
changetwokeydword   12,08000000h,13,08000000h
changetwokeydword   16,04000000h,19,00800000h
changetwokeydword   22,00000008h,24,02000000h
changetwokeydword   25,04000000h,28,00400000h
changetwokeydword   29,10000000h,32,40000000h
endm

change26   macro
changetwokeydword    2,02000000h, 4,00000002h
changetwokeydword    7,00400000h,10,40000000h
changetwokeydword   11,80000000h,14,80000000h
changetwokeydword   15,00200000h,18,00800000h
changetwokeydword   20,00000001h,23,00100000h
changethreekeydword 27,00800000h,30,00000008h,31,00080000h
endm

change27   macro
changetwokeydword    1,00100000h, 4,04000000h
changetwokeydword    6,00000004h, 7,20000000h
changetwokeydword    9,00080000h,12,00000002h
changetwokeydword   15,00400000h,17,10000000h
changetwokeydword   20,01000000h,21,00040000h
changetwokeydword   23,40000000h,26,00800000h
changetwokeydword   28,00000001h,31,08000000h
endm

change28   macro
changetwokeydword    2,00040000h, 3,00002000h
changetwokeydword    6,00000400h, 8,00008000h
changetwokeydword    9,00001000h,14,00080000h
changetwokeydword   16,00000200h,19,00000010h
changetwokeydword   22,00000080h,23,00000004h
changethreekeydword 25,00008000h,28,00000100h,32,00000040h
endm

change29   macro
changetwokeydword    1,00000400h, 4,00008000h
changetwokeydword    5,00001000h,10,00080000h
changetwokeydword   12,00000200h,14,00010000h
changetwokeydword   18,00000080h,19,00000004h
changetwokeydword   21,00008000h,24,00000100h
changethreekeydword 28,00040000h,29,00000020h,32,00000400h
endm

change30   macro
changetwokeydword    1,00004000h, 4,00000200h
changetwokeydword    6,00010000h, 9,00000800h
changetwokeydword   12,00020000h,13,00000008h
changetwokeydword   16,00004000h,20,00040000h
changetwokeydword   21,00000020h,23,00000400h
changetwokeydword   25,00000080h,28,00000800h
changetwokeydword   29,00000040h,32,00080000h
endm

change31   macro
changetwokeydword    1,00000004h, 4,00020000h
changetwokeydword    5,00000008h, 8,00004000h
changetwokeydword   10,00000040h,11,00002000h
changetwokeydword   14,00000400h,16,00008000h
changetwokeydword   17,00000080h,20,00000800h
changetwokeydword   21,00000040h,23,00004000h
changethreekeydword 27,00000010h,30,00000080h,31,00000800h
endm

change33   macro
changetwokeydword    1,80000000h, 3,00040000h
changetwokeydword    5,40000000h, 8,00800000h
changetwokeydword   10,00000001h,13,00100000h
changetwokeydword   18,00000004h,19,20000000h
changetwokeydword   21,00080000h,24,00000002h
changethreekeydword 27,00400000h,30,40000000h,32,01000000h
endm

change34   macro
changetwokeydword    2,00000002h, 3,04000000h
changetwokeydword    6,00400000h, 7,10000000h
changetwokeydword   10,01000000h,11,00040000h
changetwokeydword   13,40000000h,16,00800000h
changetwokeydword   20,08000000h,21,08000000h
changetwokeydword   24,04000000h,26,00000004h
changethreekeydword 27,20000000h,29,00080000h,32,02000000h
endm

change35   macro
changetwokeydword    2,04000000h, 5,00800000h
changetwokeydword    8,00000008h,10,02000000h
changetwokeydword   11,04000000h,14,00400000h
changetwokeydword   15,10000000h,18,40000000h
changetwokeydword   19,80000000h,22,80000000h
changetwokeydword   23,00200000h,28,08000000h
changetwokeydword   29,08000000h,31,00100000h
endm

change36   macro
changetwokeydword    1,00200000h, 4,00800000h
changetwokeydword    6,00000001h, 9,00100000h
changetwokeydword   13,00800000h,16,00000008h
changetwokeydword   17,00080000h,20,00000002h
changetwokeydword   23,00400000h,26,40000000h
changethreekeydword 27,80000000h,30,80000000h,31,40000000h
endm

change37   macro
changetwokeydword    2,00008000h, 3,00000080h
changetwokeydword    6,00000800h, 7,00000040h
changetwokeydword    9,00004000h,13,00000010h
changetwokeydword   16,00000080h,17,00000800h
changetwokeydword   20,00020000h,21,00000008h
changetwokeydword   24,00004000h,26,00000040h
changethreekeydword 27,00002000h,30,00000400h,31,00000400h
endm

change38   macro
changetwokeydword    2,00000200h, 5,00000010h
changetwokeydword    8,00000080h, 9,00000004h
changetwokeydword   11,00008000h,14,00000100h
changetwokeydword   18,00000040h,19,00002000h
changetwokeydword   22,00000400h,24,00008000h
changethreekeydword 25,00001000h,30,00080000h,31,00004000h
endm

change39   macro
changetwokeydword    2,00020000h, 3,00008000h
changetwokeydword    6,00000100h,10,00040000h
changetwokeydword   11,00000020h,13,00000400h
changetwokeydword   15,00000080h,17,00001000h
changetwokeydword   22,00080000h,24,00000200h
changethreekeydword 26,00010000h,29,00000800h,31,00000004h
endm

change41   macro
changetwokeydword    1,00040000h, 4,80000000h
changetwokeydword    5,00200000h,10,08000000h
changetwokeydword   11,08000000h,14,04000000h
changetwokeydword   16,00000004h,17,00800000h
changetwokeydword   20,00000008h,22,02000000h
changetwokeydword   23,04000000h,26,00400000h
changethreekeydword 27,10000000h,30,01000000h,31,80000000h
endm

change42   macro
changetwokeydword    1,04000000h, 5,00400000h
changetwokeydword    8,40000000h, 9,80000000h
changetwokeydword   12,80000000h,13,00200000h
changetwokeydword   18,00000001h,21,00100000h
changetwokeydword   25,00800000h,28,00000008h
changetwokeydword   30,02000000h,32,00000002h
endm

change43   macro
changetwokeydword    4,00000004h, 5,20000000h
changetwokeydword    7,00080000h,10,00000002h
changetwokeydword   13,00400000h,16,40000000h
changetwokeydword   18,01000000h,19,00040000h
changetwokeydword   21,40000000h,24,00800000h
changethreekeydword 26,00000001h,29,00100000h,32,04000000h
endm

change44   macro
changetwokeydword    2,00800000h, 6,08000000h
changetwokeydword    7,08000000h,10,04000000h
changetwokeydword   12,00000004h,13,20000000h
changetwokeydword   15,00080000h,18,02000000h
changetwokeydword   19,04000000h,22,00400000h
changetwokeydword   23,10000000h,26,01000000h
changethreekeydword 27,00040000h,29,40000000h,31,00200000h
endm

change45   macro
changetwokeydword    1,00000080h, 3,00001000h
changetwokeydword    8,00080000h,10,00000200h
changetwokeydword   12,00010000h,15,00000800h
changetwokeydword   17,00000004h,19,00008000h
changetwokeydword   22,00000100h,26,00040000h
changethreekeydword 27,00000020h,29,00000400h,32,00008000h
endm

change46   macro
changetwokeydword    4,00010000h, 7,00000800h
changetwokeydword   10,00020000h,11,00000008h
changetwokeydword   14,00004000h,16,00000040h
changetwokeydword   18,00040000h,19,00000020h
changetwokeydword   21,00000400h,23,00000080h
changetwokeydword   26,00000800h,27,00000040h
changetwokeydword   29,00004000h,32,00000200h
endm

change47   macro
changetwokeydword    1,00008000h, 3,00000008h
changetwokeydword    6,00004000h, 8,00000040h
changetwokeydword    9,00002000h,12,00000400h
changetwokeydword   14,00008000h,15,00001000h
changetwokeydword   18,00000800h,19,00000040h
changetwokeydword   21,00004000h,25,00000010h
changethreekeydword 28,00000080h,29,00000004h,32,00020000h
endm

change49   macro
changetwokeydword    2,80000000h, 3,40000000h
changetwokeydword    6,00800000h, 8,00000001h
changetwokeydword   11,00100000h,15,00800000h
changetwokeydword   17,20000000h,19,00080000h
changetwokeydword   22,00000002h,25,00400000h
changethreekeydword 28,40000000h,29,80000000h,31,00040000h
endm

change50   macro
changetwokeydword    4,00400000h, 5,10000000h
changetwokeydword    8,01000000h, 9,00040000h
changetwokeydword   11,40000000h,14,00800000h
changetwokeydword   16,00000001h,18,08000000h
changetwokeydword   19,08000000h,22,04000000h
changetwokeydword   24,00000004h,25,20000000h
changethreekeydword 27,00080000h,30,00000002h,31,04000000h
endm

change51   macro
changetwokeydword    2,00000004h, 3,00800000h
changetwokeydword    6,00000008h, 8,02000000h
changetwokeydword    9,04000000h,12,00400000h
changetwokeydword   13,10000000h,16,01000000h
changetwokeydword   17,80000000h,20,80000000h
changetwokeydword   21,00200000h,26,08000000h
changetwokeydword   27,08000000h,30,04000000h
endm

change52   macro
changetwokeydword    4,00000001h, 7,00100000h
changetwokeydword   11,00800000h,14,00000008h
changetwokeydword   16,02000000h,18,00000002h
changetwokeydword   21,00400000h,24,40000000h
changetwokeydword   25,80000000h,28,80000000h
changetwokeydword   29,00200000h,32,00800000h
endm

change53   macro
changetwokeydword    1,00001000h, 4,00000800h
changetwokeydword    5,00000040h, 7,00004000h
changetwokeydword   11,00000010h,14,00000080h
changetwokeydword   15,00000004h,18,00020000h
changetwokeydword   19,00000008h,22,00004000h
changetwokeydword   24,00000040h,25,00002000h
changethreekeydword 28,00000400h,30,00008000h,31,00000080h
endm

change54   macro
changetwokeydword    2,00010000h, 3,00000010h
changetwokeydword    6,00000080h, 7,00000004h
changetwokeydword    9,00008000h,12,00000100h
changetwokeydword   16,00040000h,17,00002000h
changetwokeydword   20,00000400h,22,00008000h
changethreekeydword 23,00001000h,28,00080000h,30,00000200h
endm

change55   macro
changetwokeydword    1,00000008h, 4,00000100h
changetwokeydword    8,00040000h, 9,00000020h
changetwokeydword   11,00000400h,13,00000080h
changetwokeydword   16,00000800h,20,00080000h
changetwokeydword   22,00000200h,24,00010000h
changethreekeydword 27,00000800h,30,00020000h,31,00008000h
endm

change57   macro
changetwokeydword    1,40000000h, 3,00200000h
changetwokeydword    8,08000000h, 9,08000000h
changetwokeydword   12,04000000h,14,00000004h
changetwokeydword   15,20000000h,18,00000008h
changetwokeydword   20,02000000h,21,04000000h
changetwokeydword   24,00400000h,25,10000000h
changethreekeydword 28,01000000h,29,00040000h,32,80000000h
endm

change58   macro
changetwokeydword    2,00400000h, 3,00400000h
changetwokeydword    6,40000000h, 7,80000000h
changetwokeydword   10,80000000h,11,00200000h
changetwokeydword   16,08000000h,19,00100000h
changetwokeydword   23,00800000h,26,00000008h
changetwokeydword   28,02000000h,29,04000000h
endm

change59   macro
changetwokeydword    1,00800000h, 3,20000000h
changetwokeydword    5,00080000h, 8,00000002h
changetwokeydword   11,00400000h,14,40000000h
changetwokeydword   15,80000000h,17,00040000h
changetwokeydword   19,40000000h,22,00800000h
changethreekeydword 24,00000001h,27,00100000h,32,00000004h
endm

change60   macro
changetwokeydword    2,00000001h, 4,08000000h
changetwokeydword    5,08000000h, 8,04000000h
changetwokeydword   10,00000004h,11,20000000h
changetwokeydword   13,00080000h,16,00000002h
changetwokeydword   17,04000000h,20,00400000h
changetwokeydword   21,10000000h,24,01000000h
changethreekeydword 25,00040000h,27,40000000h,30,00800000h
endm

change61   macro
changetwokeydword    2,00000800h, 6,00080000h
changetwokeydword    8,00000200h,10,00010000h
changetwokeydword   13,00000800h,16,00020000h
changetwokeydword   17,00008000h,20,00000100h
changetwokeydword   24,00040000h,25,00000020h
changethreekeydword 27,00000400h,29,00000080h,31,00001000h
endm

change62   macro
changetwokeydword    1,00000010h, 5,00000800h
changetwokeydword    8,00020000h, 9,00000008h
changetwokeydword   12,00004000h,14,00000040h
changetwokeydword   15,00002000h,17,00000020h
changetwokeydword   19,00000400h,21,00000080h
changetwokeydword   24,00000800h,25,00000040h
changetwokeydword   27,00004000h,32,00010000h
endm

change63   macro
changetwokeydword    2,00000100h, 4,00004000h
changetwokeydword    6,00000040h, 7,00002000h
changetwokeydword   10,00000400h,12,00008000h
changetwokeydword   13,00001000h,17,00000040h
changetwokeydword   19,00004000h,23,00000010h
changetwokeydword   26,00000080h,27,00000004h
changetwokeydword   29,00008000h,31,00000008h
endm

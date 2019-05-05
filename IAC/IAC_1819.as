;-------------------------------------------------------------------------------
;   CONSTANTES
;-------------------------------------------------------------------------------

SP_INICIAL	    EQU		FDFFh

DISPLAY         EQU     FFF0h
LCD_CONTROL     EQU     FFF4h
LCD_WRITE       EQU     FFF5h
TIMER_COUNTER   EQU     FFF6h
TIMER_CONTROL   EQU     FFF7h
BUTTONS         EQU     FFFAh
BUTTONS_MASK    EQU     1001001111111111b
JT_CURSOR       EQU     FFFCh
JT_WRITE        EQU     FFFEh
JT_CURSOR_MASK  EQU     FFFFh

ESCALA_Y        EQU     18
ESCALA_X        EQU     11

GRAVIDADE	    EQU 	0000000001001110b	    ; 8 bitas para a parte fraccionaria
PI              EQU 	0011001001000011b	; 12 bits para a
PI_QUADRADO 	EQU 	1001110111101001b	; parte fraccionaria


;-------------------------------------------------------------------------------
;   VARIAVEIS
;-------------------------------------------------------------------------------

                ORIG    8000h

JT_POSICAO      WORD    0
LCD_POSICAO     WORD    0

RANDOM_MASK     WORD    1000000000010110b
RANDOM_NI       WORD    0720h

CARACT_FINAL    STR     '@'

BORDER_1      	STR     '/--------- PLAYER 1:   ---------------------------------- PLAYER 2:   ---------\@'
BORDER_2        STR 	'|                                                                              |@'
BORDER_3        STR 	'\------------------------------ Press IC to exit ------------------------------/@'

LCD_ANGULO      STR     'Angulo:        @'
LCD_ANG_DFLT    WORD    800Eh
LCD_VELOCIDADE 	STR     'Velocidade:    @'
LCD_VEL_DFLT    WORD    801Dh

GORILA_1        STR     'd**b@'
GORILA_2        STR     '/||\@'
GORILA_3        STR     ' /\ @'
GORILA_4        STR     '    @'

GAMEOVER_1      STR     '/--------------------------------------------------------------\@'
GAMEOVER_2      STR     '| ###### ###### ##    ## #####   ###### ##     ## ##### ###### |@'
GAMEOVER_3      STR     '| ##     ##  ## ###  ### ##      ##  ##  ##   ##  ##    ##  ## |@'
GAMEOVER_4      STR     '| ## ### ###### ## ## ## #####   ##  ##   ## ##   ##### ####   |@'
GAMEOVER_5      STR     '| ##  ## ##  ## ##    ## ##      ##  ##    ###    ##    ## ##  |@'
GAMEOVER_6      STR     '| ###### ##  ## ##    ## #####   ######     #     ##### ##  ## |@'
GAMEOVER_7      STR     '\--------------------------------------------------------------/@'

POS_JOGADOR     WORD    0
POS_INI_PROJ    WORD    0
POS_ALVO        WORD    0

JOGADOR_ATUAL   WORD    2
PONTOS_JOG1     WORD    0
PONTOS_JOG2     WORD    0

ANGULO          WORD    0
ANG_RAD         WORD 	0
COS_ANG         WORD	0
SIN_ANG         WORD	0

VELOCIDADE      WORD    0

TEMPO           WORD    0

POS_PROJ_Y      WORD    0
POS_PROJ_X      WORD    0
POS_ANT_PROJ    WORD    000Ah
POS_PROJ        WORD    0


; ------------------------------------------------------------------------------
;	Tabela de  Interrupcoes
; ------------------------------------------------------------------------------

                ORIG    FE00h

BUTAO_0         WORD    INATIVO
BUTAO_1         WORD    INATIVO
BUTAO_2         WORD    INATIVO
BUTAO_3         WORD    INATIVO
BUTAO_4         WORD    INATIVO
BUTAO_5         WORD    INATIVO
BUTAO_6         WORD    INATIVO
BUTAO_7         WORD    INATIVO
BUTAO_8         WORD    INATIVO
BUTAO_9         WORD    INATIVO


                ORIG    FE0Ch

BUTAO_C         WORD    EXIT


                ORIG    FE0Fh

TIMER           WORD    INATIVO


; ############################################################################ ;


                ORIG    0000h

                MOV     R1, SP_INICIAL
                MOV     SP, R1

                MOV     R1, JT_CURSOR_MASK
                MOV     M[JT_CURSOR], R1

                MOV     R1, BUTTONS_MASK
                MOV     M[BUTTONS], R1
                ENI

                MOV     R1, LCD_POSICAO
                MOV     M[LCD_CONTROL], R1

                JMP     cicloJogo


;-------------------------------------------------------------------------------
;	INTERRUPCOES
;-------------------------------------------------------------------------------

BUTAO_0F:       MOV     R1, 0
                DEC     R2
                RTI

BUTAO_1F:       MOV     R1, 1
                DEC     R2
                RTI

BUTAO_2F:       MOV     R1, 2
                DEC     R2
                RTI

BUTAO_3F:       MOV     R1, 3
                DEC     R2
                RTI

BUTAO_4F:       MOV     R1, 4
                DEC     R2
                RTI

BUTAO_5F:       MOV     R1, 5
                DEC     R2
                RTI

BUTAO_6F:       MOV     R1, 6
                DEC     R2
                RTI

BUTAO_7F:       MOV     R1, 7
                DEC     R2
                RTI

BUTAO_8F:       MOV     R1, 8
                DEC     R2
                RTI

BUTAO_9F:       MOV     R1, 9
                DEC     R2
                RTI

EXIT:           JMP     finalJogo
                RTI

TIMERF:         MOV     R3, 1
                MOV     M[TIMER_COUNTER], R3
                MOV     R3, 1
                MOV     M[TIMER_CONTROL], R3
                SHR     R4, 1
                RTI

INATIVO:        RTI


; ------------------------------------------------------------------------------
;	FUNCOES
; ------------------------------------------------------------------------------

; ------------------------------------------------------------------------------
;	Avanca uma posicao na janela de texto

proxCaractIO:   PUSH    R1
                INC     M[JT_POSICAO]
                MOV     R1, M[JT_POSICAO]
                MOV     M[JT_CURSOR], R1
                POP     R1
                RET

; ------------------------------------------------------------------------------
;	Avanca uma linha na janela de texto
;		- recebe como argumento o numero de linhas
;		- que se deseja recuar na linha seguinte

proxLinhaIO:    PUSH    R1
                MOV     R1, M[JT_POSICAO]
                ADD     R1, 0100h
                SUB     R1, M[SP+3]
                MOV     M[JT_POSICAO], R1
                MOV     M[JT_CURSOR], R1
                POP     R1
                RETN    1

; ------------------------------------------------------------------------------
;	Escreve uma string na janela de texto
;		- recebe como argumento a string que se deseja escrever
;		- e a posicao onde deve comecar a ser escrita

escreveIO:      PUSH    R1
                PUSH    R2
                PUSH    R3
                PUSH    R4
                MOV     R1, M[SP+6]
                MOV     M[JT_POSICAO], R1
                MOV     M[JT_CURSOR], R1
                MOV     R2, M[CARACT_FINAL]
                MOV     R3, M[SP+7]
escreveIOAux:   MOV     R4, M[R3]
                CMP     R2, R4
                BR.Z    escreveIOFim
                MOV     M[JT_WRITE], R4
                CALL    proxCaractIO
                INC     R3
                BR      escreveIOAux

escreveIOFim:   POP     R4
                POP     R3
                POP     R2
                POP     R1
                RETN    2

; ------------------------------------------------------------------------------

proxCaractLCD:  PUSH    R1
                INC     M[LCD_POSICAO]
                MOV     R1, M[LCD_POSICAO]
                MOV     M[LCD_CONTROL], R1
                POP     R1
                RET

; ------------------------------------------------------------------------------

escreveLCD:     PUSH    R1
                PUSH    R2
                PUSH    R3
                PUSH    R4
                MOV     R1, M[SP+6]
                MOV     M[LCD_POSICAO], R1
                MOV     M[LCD_CONTROL], R1
                MOV     R2, M[CARACT_FINAL]
                MOV     R3, M[SP+7]
escreveLCDAux:  MOV     R4, M[R3]
                CMP     R2, R4
                BR.Z    escreveLCDFim
                MOV     M[LCD_WRITE], R4
                CALL    proxCaractLCD
                INC     R3
                BR      escreveLCDAux

escreveLCDFim:  POP     R4
                POP     R3
                POP     R2
                POP     R1
                RETN    2

;-------------------------------------------------------------------------------

anguloLCD:      PUSH    R1
                PUSH    LCD_ANGULO
                PUSH    8000h
                CALL    escreveLCD
                MOV     R1, 800Fh
                MOV     M[LCD_CONTROL], R1
                MOV     R1, 32
                MOV     M[LCD_WRITE], R1
                POP     R1
                RET

; ------------------------------------------------------------------------------

velocidadeLCD:  PUSH    R1
                PUSH    LCD_VELOCIDADE
                PUSH    8010h
                CALL    escreveLCD
                MOV     R1, 801Fh
                MOV     M[LCD_CONTROL], R1
                MOV     R1, 32
                MOV     M[LCD_WRITE], R1
                POP     R1
                RET

; ------------------------------------------------------------------------------

escreveNumLCD:  PUSH    R1
                PUSH    R2
                MOV     R1, M[SP+4]
                MOV     M[LCD_CONTROL], R1
                MOV     R2, M[SP+5]
                ADD     R2, 48
                MOV     M[LCD_WRITE], R2
                POP     R2
                POP     R1
                RETN    2

; ------------------------------------------------------------------------------

desenhaGorila:  PUSH    R1
                MOV     R1, M[SP+3]
                PUSH    GORILA_1
                PUSH    R1
                CALL    escreveIO
                PUSH    4
                CALL    proxLinhaIO
                PUSH    GORILA_2
                PUSH    M[JT_POSICAO]
                CALL    escreveIO
                PUSH    4
                CALL    proxLinhaIO
                PUSH    GORILA_3
                PUSH    M[JT_POSICAO]
                CALL    escreveIO
                POP     R1
                RETN    1

; ------------------------------------------------------------------------------

aleatoria:      PUSH    R1
                MOV     R1, M[RANDOM_NI]
                AND     R1, FFFEh
                CMP     R1, R0
                BR.NZ   aleatoriaElse
                MOV     R1, M[RANDOM_NI]
                ROR     R1, 1
                BR      aleatoriaFim

aleatoriaElse:  MOV     R1, M[RANDOM_NI]
                XOR     R1, M[RANDOM_MASK]
                ROR     R1, 1

aleatoriaFim:   MOV     M[RANDOM_NI], R1
                POP     R1
                RET

; ------------------------------------------------------------------------------

geraPos:        PUSH    R1
                PUSH    R2

geraPosY:       CALL    aleatoria
                MOV     R1, M[RANDOM_NI]
                MOV     R2, 20
                DIV     R1, R2
                INC     R2
                SHL     R2, 8
                PUSH    R2

geraPosX:       CALL    aleatoria
                MOV     R1, M[RANDOM_NI]
                MOV     R2, 15
                DIV     R1, R2
                INC     R2

                MOV     R1, M[SP+5]
                CMP     R1, 1
                BR.Z    geraPosJogador

geraPosAlvo:    ADD     R2, 60
                POP     R1
                ADD     R1, R2
                MOV     M[POS_ALVO], R1
                PUSH    R1
                CALL    desenhaGorila
                BR      geraPosFim

geraPosJogador: POP     R1
                ADD     R1, R2
                MOV     M[POS_JOGADOR], R1
                PUSH    R1
                CALL    desenhaGorila
                ADD     R1, 0103h
                MOV     M[POS_INI_PROJ], R1

geraPosFim:     POP     R2
                POP     R1
                RETN    1

; ------------------------------------------------------------------------------

geraGorilas:    PUSH    1
                CALL    geraPos
                PUSH    2
                CALL    geraPos
                RET

; ------------------------------------------------------------------------------

ativaButoes:    PUSH    R1
                MOV     R1, BUTAO_0F
                MOV     M[BUTAO_0], R1
                MOV     R1, BUTAO_1F
                MOV     M[BUTAO_1], R1
                MOV     R1, BUTAO_2F
                MOV     M[BUTAO_2], R1
                MOV     R1, BUTAO_3F
                MOV     M[BUTAO_3], R1
                MOV     R1, BUTAO_4F
                MOV     M[BUTAO_4], R1
                MOV     R1, BUTAO_5F
                MOV     M[BUTAO_5], R1
                MOV     R1, BUTAO_6F
                MOV     M[BUTAO_6], R1
                MOV     R1, BUTAO_7F
                MOV     M[BUTAO_7], R1
                MOV     R1, BUTAO_8F
                MOV     M[BUTAO_8], R1
                MOV     R1, BUTAO_9F
                MOV     M[BUTAO_9], R1
                POP     R1
                RET

; ------------------------------------------------------------------------------

desativaButoes: PUSH    R1
                MOV     R1, INATIVO
                MOV     M[BUTAO_0], R1             ; Desativa as BUTAO_erupcoes desnecessarias
                MOV     M[BUTAO_1], R1                             ; para nao BUTAO_erferirem com o funcionamento
                MOV     M[BUTAO_2], R1                             ; do programa
                MOV     M[BUTAO_3], R1
                MOV     M[BUTAO_4], R1
                MOV     M[BUTAO_5], R1
                MOV     M[BUTAO_6], R1
                MOV     M[BUTAO_7], R1
                MOV     M[BUTAO_8], R1
                MOV     M[BUTAO_9], R1
                POP     R1
                RET

; ------------------------------------------------------------------------------

insereAng:      PUSH    R1
                PUSH    R2
                PUSH    R3
                PUSH    R4
                PUSH    R5
                CALL    ativaButoes

                MOV     R2, 2
                MOV     R3, R0
                MOV     R5, M[LCD_ANG_DFLT]
insereAngCiclo: MOV     R4, 10
                CMP     R2, R0
                BR.Z    insereAngFim

                MOV     R1, -1
inputAng:       CMP     R1, R0					; O programa fica num loop enquanto nenhuma
                BR.N    inputAng  					; interrupcao de I1 a I6 é ativa
                MUL     R4, R3
                PUSH    R1
                PUSH    R5
                CALL    escreveNumLCD
                ADD     R3, R1
                INC     R5
                BR      insereAngCiclo

insereAngFim:   CALL    desativaButoes
                MOV     M[ANGULO], R3
                POP     R5
                POP     R4
                POP     R3
                POP     R2
                POP     R1
                RET

; ------------------------------------------------------------------------------

insereVel:      PUSH    R1
                PUSH    R2
                PUSH    R3
                PUSH    R4
                PUSH    R5
                CALL    ativaButoes

                MOV     R2, 3
                MOV     R3, R0
                MOV     R5, M[LCD_VEL_DFLT]
insereVelCiclo: MOV     R4, 10
                CMP     R2, R0
                BR.Z    insereVelFim

                MOV     R1, -1
inputVel:       CMP     R1, R0					; O programa fica num loop enquanto nenhuma
                BR.N    inputVel  					; interrupcao de I1 a I6 é ativa
                MUL     R4, R3
                PUSH    R1
                PUSH    R5
                CALL    escreveNumLCD
                ADD     R3, R1
                INC     R5
                BR      insereVelCiclo

insereVelFim:   CALL    desativaButoes
                MOV     M[VELOCIDADE], R3
                POP     R5
                POP     R4
                POP     R3
                POP     R2
                POP     R1
                RET

; ------------------------------------------------------------------------------

insereJogada:   CALL    anguloLCD
                CALL    insereAng
                MOV     R1, M[ANGULO]
                CMP     R1, 90
                BR.P    insereJogada

insereJogada1:  CALL    velocidadeLCD
                CALL    insereVel
                MOV     R1, M[VELOCIDADE]
                CMP     R1, 120
                BR.P    insereJogada1

insereJogFim:   RET

; ------------------------------------------------------------------------------

dispJogAtual:   PUSH    R1
                MOV     R1, M[JOGADOR_ATUAL]
                MOV     M[DISPLAY], R1
                POP     R1
                RET

; ------------------------------------------------------------------------------

trocaJogador:   PUSH    R1
                MOV     R1, M[JOGADOR_ATUAL]
                CMP     R1, 1
                BR.Z    trocaJogElse
                MOV     R1, 1
                BR      trocaJogFim

trocaJogElse:   MOV     R1, 2

trocaJogFim:    MOV     M[JOGADOR_ATUAL], R1
                POP     R1
                RET

; ------------------------------------------------------------------------------

calcAngulares:  PUSH    R1
                PUSH    R2
                PUSH    R3
                PUSH    R4

converteAng:	MOV 	R1, M[ANGULO]
                MOV 	R2, 180
                SHL 	R1, 8
                DIV 	R1, R2
                MOV 	R2, PI
                MUL 	R2, R1
                SHR 	R1, 8
                SHL 	R2, 8
                ADD 	R1, R2
                MOV 	M[ANG_RAD], R1

sin:            MOV 	R2, M[ANG_RAD]
                MOV 	R1, PI
                SUB 	R1, R2				; R1 -> (pi - x) 12 bits fraccionaria
                PUSH 	R1
                MOV 	R3, 16
                MUL 	R3, R2
                SHL 	R3, 12
                SHR 	R2, 4
                ADD 	R2, R3 				; R2 -> (16 * x) 8 bits fraccionaria
                MUL 	R1, R2
                SHL 	R1, 4
                SHR 	R2, 12
                ADD 	R1, R2 				; R1 -> 16 * x * (pi - x)
                MOV 	R2, 5
                MOV 	R3, PI_QUADRADO
                MUL  	R2, R3
                SHL 	R2, 12
                SHR 	R3, 4
                ADD 	R2, R3 				; R2 -> (5 * pi²) 8 bits
                MOV 	R3, 4
                MOV 	R4, M[ANG_RAD]
                MUL 	R4, R3				; R3 -> (4 * x) 12 bits
                POP 	R4
                MUL 	R3, R4				; R3 -> 4 * x * (pi - x) 8 bits
                SUB 	R2, R3				; R2 -> 5 * pi² - 4 * x * (pi - x) 8 bits
                SHR 	R2, 8
                DIV 	R1, R2
                MOV 	M[SIN_ANG], R1

cos:            MOV 	R1, M[ANG_RAD]
                MOV 	R2, M[ANG_RAD]
                MUL 	R1, R2
                SHL     R1, 4
                SHR 	R2, 12
                ADD     R1, R2 				; R1 -> (x²) 12 bits
                PUSH  	R1
                MOV 	R2, 4
                MUL 	R1, R2
                MOV 	R1, PI_QUADRADO
                SUB 	R1, R2				; R1 -> (pi² - 4x²) 12 bits
                MOV 	R2, PI_QUADRADO
                POP 	R3
                ADD 	R2, R3				; R2 -> (pi² + x²) 12 bits
                SHR 	R2, 8
                DIV 	R1, R2
                MOV 	M[COS_ANG], R1		; Cosseno com 8 bits fraccionaria

calcAngFim:     POP     R4
                POP     R3
                POP     R2
                POP     R1
                RET

; ------------------------------------------------------------------------------

calcPos:        PUSH    R1
                PUSH    R2
                PUSH    R3

movimentoY:     MOV 	R1, M[VELOCIDADE]
                MOV 	R2, M[SIN_ANG]
                MUL 	R2, R1
                SHR     R1, 4
                SHL     R2, 12
                ADD 	R1, R2
                MOV 	R2, M[TEMPO]
                MUL 	R2, R1
                SHR     R1, 4
                SHL     R2, 12
                ADD 	R1, R2
                MOV 	R2, M[TEMPO]
                MOV 	R3, M[TEMPO]
                MUL 	R3, R2
                SHR     R2, 4
                SHL 	R3, 12
                ADD     R2, R3
                MOV 	R3, GRAVIDADE  	  ; Calculei previamente o meio da
                MUL 	R3, R2            ; forca gravitica para facilitar as contas
                SHR     R2, 4
                SHL 	R3, 12
                ADD     R2, R3
                PUSH    R2
addPosIniY:     MOV     R2, M[POS_INI_PROJ]
                AND     R2, FF00h
                MOV     R3, 1700h
                SUB     R3, R2
                SHR     R3, 8
                MOV     R2, ESCALA_Y
                MUL     R2, R3
                SHL     R3, 4
                ADD     R1, R3
                POP     R2
                SUB     R1, R2
                MOV     M[POS_PROJ_Y], R1
convertePosY:   MOV     R2, ESCALA_Y
                DIV     R1, R2
                SHR     R1, 4
                MOV     R2, 24
                SUB     R2, R1
                SHL     R2, 8
                PUSH    R2

movimentoX:     MOV 	R1, M[VELOCIDADE]
                MOV 	R2, M[COS_ANG]
                MUL 	R2, R1
                SHR     R1, 4
                SHL     R2, 12
                ADD 	R1, R2
                MOV 	R2, M[TEMPO]
                MUL 	R2, R1
                SHR     R1, 4
                SHL     R2, 12
                ADD     R1, R2
addPosIniX:     MOV     R2, M[POS_INI_PROJ]
                AND     R2, 00FFh
                MOV     R3, ESCALA_X
                MUL     R3, R2
                SHL     R2, 4
                ADD     R1, R2
                MOV     M[POS_PROJ_X], R1
convertePosX:   MOV     R2, ESCALA_X
                DIV     R1, R2
                SHR     R1, 4
                POP     R2
                ADD     R1, R2
                MOV     M[POS_PROJ], R1

calcPosFim:     POP     R3
                POP     R2
                POP     R1
                RET

; ------------------------------------------------------------------------------

desenhaProj:	PUSH 	R1
                PUSH 	R2
                PUSH 	R3
                MOV 	R1, M[POS_PROJ]
                MOV 	R2, M[POS_ANT_PROJ]
                CMP 	R1, R2
                BR.Z 	desProjFim
                AND     R1, FF00h
                CMP     R1, R0
                BR.Z    desProjFim
                MOV     R1, M[POS_PROJ]
                MOV 	M[JT_CURSOR], R1
                MOV 	R3, '('
                MOV 	M[JT_WRITE], R3
                MOV 	M[JT_CURSOR], R2
                MOV 	R3, ' '
                MOV 	M[JT_WRITE], R3
                MOV 	M[POS_ANT_PROJ], R1

desProjFim:	    POP 	R3
                POP 	R2
                POP 	R1
                RET

; ------------------------------------------------------------------------------


hit:            PUSH    R1
                PUSH    R2
                PUSH    R3
                PUSH    R4
                MOV     R1, M[POS_PROJ]
                MOV     R2, M[POS_ALVO]

hitVerifica:    CMP     R1, R2
                JMP.N   hitFim
                MOV     R3, R2
                ADD     R3, 0203h
                CMP     R1, R3
                BR.P    hitFim
                MOV     R3, R1
                AND     R3, 00FFh
                MOV     R4, R2
                AND     R4, 00FFh
                CMP     R3, R4
                BR.N    hitFim
                ADD     R4, 0003h
                CMP     R3, R4
                BR.P    hitFim
                MOV     R1, 1
                MOV     M[SP+6], R1

hitFim:         POP     R4
                POP     R3
                POP     R2
                POP     R1
                RET

; ------------------------------------------------------------------------------

calcProjetil:   PUSH    R1
                PUSH    R2
                PUSH	R3
                PUSH	R4
                MOV     R1, TIMERF
                MOV     M[TIMER], R1
                MOV     R1, 1
                MOV     M[TIMER_COUNTER], R1
                MOV     R1, 1
                MOV     M[TIMER_CONTROL], R1

                MOV     M[TEMPO], R0

calcProjCiclo:  MOV 	R4, 0001h
calcProjAux:	CMP 	R4, R0
                BR.NZ	calcProjAux
                MOV 	R4, 100b
                ADD     M[TEMPO], R4
                CALL 	calcPos
                MOV 	R1, M[POS_PROJ]
                AND 	R1, 00FFh
                MOV 	R2, 004Fh
                CMP 	R1, R2
                MOV     R1, 0
                BR.Z 	calcProjFim
                MOV 	R1, M[POS_PROJ]
                AND 	R1, FF00h
                MOV 	R2, 1700h
                CMP 	R1, R2
                MOV     R1, 0
                BR.Z 	calcProjFim
                CALL 	desenhaProj
                PUSH    R0
                CALL    hit
                POP     R1
                CMP     R1, 1
                BR.Z    calcProjFim
                JMP     calcProjCiclo

calcProjFim:    MOV     M[SP+6], R1
                MOV     R1, INATIVO
                MOV     M[TIMER], R1
                POP 	R4
                POP 	R3
                POP     R2
                POP     R1
                RET

; ##############################################################################

; ------------------------------------------------------------------------------
;	Imprime uma borda na janela de texto
;		- (efeito estetico)

border:         PUSH    R1
                MOV     R1, 22
                PUSH    BORDER_1
                PUSH    0000h
                CALL    escreveIO

borderCiclo:    PUSH    80
                CALL    proxLinhaIO
                CMP     R1, R0
                BR.Z    borderFim
                PUSH    BORDER_2
                PUSH    M[JT_POSICAO]
                CALL    escreveIO
                DEC     R1
                BR      borderCiclo

borderFim:      PUSH 	BORDER_3
                PUSH	M[JT_POSICAO]
                CALL 	escreveIO
                POP     R1
                RET

; ------------------------------------------------------------------------------

score:          PUSH    R1
                MOV     R1, 0015h
                MOV     M[JT_CURSOR], R1
                MOV     R1, M[PONTOS_JOG1]
                ADD     R1, 48
                MOV     M[JT_WRITE], R1
                MOV     R1, 0044h
                MOV     M[JT_CURSOR], R1
                MOV     R1, M[PONTOS_JOG2]
                ADD     R1, 48
                MOV     M[JT_WRITE], R1
                POP     R1
                RET

; ------------------------------------------------------------------------------
;	Imprime uma mensagem de GAME OVER
;		- (efeito estetico)

gameover:       PUSH    GAMEOVER_1
                PUSH    0708h
                CALL    escreveIO
                PUSH    GAMEOVER_2
                PUSH    0808h
                CALL    escreveIO
                PUSH    GAMEOVER_3
                PUSH    0908h
                CALL    escreveIO
                PUSH    GAMEOVER_4
                PUSH    0A08h
                CALL    escreveIO
                PUSH    GAMEOVER_5
                PUSH    0B08h
                CALL    escreveIO
                PUSH    GAMEOVER_6
                PUSH    0C08h
                CALL    escreveIO
                PUSH    GAMEOVER_7
                PUSH    0D08h
                CALL    escreveIO
                RET

; ------------------------------------------------------------------------------
;    INICIO DO JOGO
; ------------------------------------------------------------------------------


cicloJogo:      CALL    border
                CALL    score
                CALL    geraGorilas
                CALL    trocaJogador
novaTentativa:  CALL    anguloLCD
                CALL    velocidadeLCD
                CALL    dispJogAtual
                CALL    insereJogada
                CALL    calcAngulares
                PUSH    R0
                CALL    calcProjetil
                POP     R1
                CMP     R1, 1
                BR.Z    adicionaPonto
                CALL    trocaJogador
                BR      novaTentativa

adicionaPonto:  MOV     R1, M[JOGADOR_ATUAL]
                CMP     R1, 1
                BR.Z    adicionaPonto1

adicionaPonto2: INC     M[PONTOS_JOG2]
                JMP     cicloJogo

adicionaPonto1: INC     M[PONTOS_JOG1]
                JMP     cicloJogo

finalJogo:      CALL    gameover

fim:            BR      fim

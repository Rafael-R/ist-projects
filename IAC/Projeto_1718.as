;-----------------------------------------------------------------------------
;Projeto realizado por: (Grupo 20)
;- Rafael Rodrigues, N 89532
;- Vasco Faria, N 89559
;-----------------------------------------------------------------------------

;#############################################################################

;-----------------------------------------------------------------------------
;   Constantes:
;-----------------------------------------------------------------------------

SP_INICIAL          EQU     FDFFh
DISPLAY7_0          EQU     FFF0h
DISPLAY7_1          EQU     FFF1h
DISPLAY7_2          EQU     FFF2h
DISPLAY7_3          EQU     FFF3h
LCD_CONTROL         EQU     FFF4h
LCD_WRITE           EQU     FFF5h
TIMER_COUNTER       EQU     FFF6h
TIMER_CONTROL       EQU     FFF7h
LEDS                EQU     FFF8h
INT_CONTROL         EQU     FFFAh
JT_CONTROL          EQU     FFFCh
JT_WRITE            EQU     FFFEh
ATIVA_INT           EQU     1000010001111110b
ATIVA_LED           EQU     1111111111111111b
ATIVA_LCD           EQU     1000000000000000b

;-----------------------------------------------------------------------------
;   Variaveis:
;-----------------------------------------------------------------------------

					ORIG    8000h
CARACT_FINAL        STR     '@'
MSG_INICIAL         STR     'Carregue no Botao IA para iniciar@'
MSG_FINAL_1         STR     'Fim do Jogo@'
MSG_FINAL_2         STR     'Carregue no Botao IA para recomecar@'
MSG_DISPLAY         STR     'Recorde: @'
RANDOM_MASK         WORD    1000000000010110b
RANDOM_STATE        WORD    0B31h
NI_1                WORD    0000h
NUM_GER             WORD    0001h
NUM_INS             WORD    0002h
REP_INT             WORD    0000h
COUNTER_XO          WORD    0000h
NUM_TENT            WORD    0
MAX_TENT            WORD    12
MELHOR_TENT         WORD    13
ATIVA_JT            WORD    0

;-----------------------------------------------------------------------------
;	Tabela de  Interrupcoes:
;-----------------------------------------------------------------------------

					ORIG    FE01h
INT1                WORD    INT_DUMMIES
INT2                WORD    INT_DUMMIES
INT3                WORD    INT_DUMMIES
INT4                WORD    INT_DUMMIES
INT5                WORD    INT_DUMMIES
INT6                WORD    INT_DUMMIES

					ORIG    FE0Ah
INTA                WORD    INT_DUMMIES

					ORIG    FE0Fh
INTF                WORD    INT_DUMMIES

					ORIG    0000h
					JMP     INICIO

;-----------------------------------------------------------------------------
;	Interrupcoes:
;-----------------------------------------------------------------------------

INT1F:              ADD     R4, 0001h
					DEC     R5
					RTI
					
INT2F:              ADD     R4, 0002h
					DEC     R5
					RTI
					
INT3F:              ADD     R4, 0003h
					DEC     R5
					RTI
					
INT4F:              ADD     R4, 0004h
					DEC     R5
					RTI
					
INT5F:              ADD     R4, 0005h
					DEC     R5
					RTI
					
INT6F:              ADD     R4, 0006h
					DEC     R5
					RTI

INTAF:              INC     R4
					RTI

CRONOMETRO:         MOV     R1, 5
					MOV     M[TIMER_COUNTER], R1
                    MOV     R1, 1
					MOV     M[TIMER_CONTROL], R1
					SHR     R2, 1
					RTI
					
INT_DUMMIES:        RTI

LCD:                MOV     R3, M[ATIVA_LCD]
					MOV     M[LCD_CONTROL], R3

;#############################################################################
;	FUNCOES:
;#############################################################################

;-----------------------------------------------------------------------------
;Aleatoria
;Descricao: Gera a sequencia aleatoria
;-----------------------------------------------------------------------------

ALEATORIA:          PUSH    R1
					PUSH    R2
					PUSH    R3
					PUSH    R4
					MOV     R3, R0
					MOV     R1, R0
					MOV     R4, M[RANDOM_STATE]
					SHR     R4, 1
					MOV     M[NI_1], R4
					BR.NC   BIT_0
					MOV     R4, M[NI_1]
					XOR     R4, M[RANDOM_MASK]
					ROR     R4, 1
					BR      CONVERS
			
BIT_0:              MOV     R4, M[NI_1]
					ROR     R4, 1
					BR      CONVERS

CONVERS:            MOV     R2, 6					; Ao dividir o numero por seis e 
					DIV     R4, R2					; incrementarmos o resto da divisao
					INC     R2						; obtemos um numero, cujos seus ultimos 
					AND     R2, 0007h				; tres digitos sao filtrados, obtendo
					SHL     R1, 3					; um numero entre 1 e 6
					ADD     R1, R2
					INC     R3
					CMP     R3, 4
					BR.Z    FIM_ALEATORIA
					BR      CONVERS

FIM_ALEATORIA:      MOV     M[NUM_GER], R1
					POP     R4
					POP     R3
					POP     R2
					POP     R1  
					RET

;-----------------------------------------------------------------------------
;Novo Jogo
;Descricao: Inicia um novo Jogo
;-----------------------------------------------------------------------------

NOVO_JOGO:          PUSH    R1
					PUSH    R2
					PUSH    R3
					PUSH    R4
					MOV 	R6, INTAF				; Ativa a interrupcao IA
					MOV     M[INTA], R6

					MOV     R3, M[CARACT_FINAL]		; Enquanto o valor de R2 nao e igual
JOGO:               MOV     R2, M[R1]				; ao caracter final(R3), o programa
					CMP     R3, R2					; escreve a mensagem
					BR.Z    BOTAO_IA
					MOV     M[JT_WRITE], R2
					CALL    NEXT_CARACT
					INC     R1
					BR      JOGO
					
BOTAO_IA:           ENI

					MOV     R4, R0
					MOV     R1, M[RANDOM_MASK]		; Enquanto o jogador nao pressionar o
CICLO_IA:           INC     R1						; botao IA a mascara e incrementada
					CMP     R4, R0					; tornando-a aleatoria
					BR.Z    CICLO_IA
					MOV     M[RANDOM_MASK], R1
					CALL    ALEATORIA
					CALL    LIMPA_JANELA

FIM_NJ:             MOV     R6, INT_DUMMIES			; Desativa a interrupcao IA
					MOV     M[INTA], R6
					POP     R4
					POP     R3
					POP     R2
					POP     R1
					RET

;-----------------------------------------------------------------------------
;Displays de 7 Segmentos
;Descricao: Displays de 7 indica a jogada atual
;-----------------------------------------------------------------------------

DISPLAY:            PUSH    R1
					PUSH    R2
					PUSH    R3
					MOV     R1, M[NUM_TENT]
					MOV     R2, 13
					CMP     R1, R2
					BR.Z    RESET
					MOV     R2, 10
					CMP     R1, R2
					BR.NN   DISPLAY_2_DIG

DISPLAY_1_DIG:      MOV     M[DISPLAY7_3], R0
                    MOV     M[DISPLAY7_2], R0
                    MOV     M[DISPLAY7_1], R0
					MOV     M[DISPLAY7_0], R1
					BR      FIM_DISPLAY

DISPLAY_2_DIG:      MOV     R3, 10					; Filtra o ultimo digito do numero
					DIV     R1, R3
					MOV     R1, 1
					MOV     M[DISPLAY7_3], R0
                    MOV     M[DISPLAY7_2], R0
					MOV     M[DISPLAY7_1], R1
					MOV     M[DISPLAY7_0], R3
					BR      FIM_DISPLAY

RESET:              MOV     M[DISPLAY7_1], R0
					MOV     M[DISPLAY7_0], R0
					BR      FIM_DISPLAY

FIM_DISPLAY:        POP     R3
					POP     R2
					POP     R1
					RET

;-----------------------------------------------------------------------------
;Numero Inserido
;Descricao: Funcao que permite inserir um numero
;-----------------------------------------------------------------------------

NUM_INSERIDO:       PUSH    R1
					PUSH    R2
					PUSH    R3
					PUSH    R4
					PUSH    R5
					PUSH    R6
					MOV 	R6, INT1F				; Ativa as interrupcoes necessarias
					MOV     M[INT1], R6
					MOV 	R6, INT2F
					MOV     M[INT2], R6
					MOV 	R6, INT3F
					MOV     M[INT3], R6
					MOV 	R6, INT4F
					MOV     M[INT4], R6
					MOV 	R6, INT5F
					MOV     M[INT5], R6
					MOV 	R6, INT6F
					MOV     M[INT6], R6
					MOV     R6, CRONOMETRO
					MOV     M[INTF], R6

TEMPORIZADOR:       MOV     R1, 5
					MOV     M[TIMER_COUNTER], R1
                    MOV     R1, 1
					MOV     M[TIMER_CONTROL], R1
					MOV     R2, ATIVA_LED

					MOV     R5, 4
					MOV     R3, R0
CICLO_JOGADA:       CMP     R5, R0
					JMP.Z   FIM_NUM_INSERIDO
					ENI

					MOV     R4, R0
INPUT:              MOV     M[LEDS], R2
					CMP     R2, R0
					JMP.Z   TIME_OUT
					CMP     R4, R0					; O programa fica num loop enquanto nenhuma
					BR.Z    INPUT  					; interrupcao de I1 a I6 é ativa
					SHL     R3, 3
					ADD     R3, R4
					BR      CICLO_JOGADA

TIME_OUT:			MOV     R6, INT_DUMMIES
					MOV     M[INT1], R6
					MOV     M[INT2], R6
					MOV     M[INT3], R6
					MOV     M[INT4], R6
					MOV     M[INT5], R6
					MOV     M[INT6], R6
					MOV     M[INTF], R6
					JMP 	MSG_F

FIM_NUM_INSERIDO:   MOV     R6, INT_DUMMIES			; Desativa as interupcoes desnecessarias
					MOV     M[INT1], R6				; para nao interferirem com o funcionamento
					MOV     M[INT2], R6				; do programa
					MOV     M[INT3], R6
					MOV     M[INT4], R6
					MOV     M[INT5], R6
					MOV     M[INT6], R6
					MOV     M[INTF], R6
					MOV     M[NUM_INS], R3
					POP     R6
					POP     R5
					POP     R4
					POP     R3
					POP     R2
					POP     R1
					RET

;-----------------------------------------------------------------------------
;Comparacao de X
;Descricao: Verifica a existencia do digitos iguais na posicao certa
;-----------------------------------------------------------------------------

COMP_X:             MOV     R1, M[NUM_GER]
					MOV     R2, M[NUM_INS]
					PUSH    R3
					PUSH    R4
					PUSH    R5
					PUSH    R1
					PUSH    R2
					MOV     M[REP_INT], R0
					MOV     M[COUNTER_XO], R0 
					
					MOV     R4, 4
LOOP_X:             CMP     R4, R0
					JMP.Z   FIM_COMP_X
					MOV     M[SP + 2], R1
					MOV     M[SP + 1], R2
					AND     R1, 0007h				; Filtra o ultimo digito de cada
					AND     R2, 0007h				; numero para serem comparados
					CMP     R1, R2
					BR.Z    SAO_X
					BR      NAO_SAO_X

SAO_X:              MOV     R3, M[REP_INT]
					SHL     R3, 3					; Insere o numero 1 na
					INC     R3						; representacao interna
					MOV     M[REP_INT], R3
					MOV     R5, M[COUNTER_XO]
					INC     R5
					MOV     M[COUNTER_XO], R5
					MOV     R1, M[SP + 2]
					MOV     R2, M[SP + 1]
					SHR     R1, 3					; Avanca 3 bits em cada numero para 
					SHR     R2, 3					; podermos comparar o proximo digito
					DEC     R4						; e substitui por zero os anteriores
					JMP     LOOP_X

NAO_SAO_X:          MOV     R1, M[SP + 2]
					MOV     R2, M[SP + 1]
					ROR     R1, 3					; Avanca 3 bits em cada numero para
					ROR     R2, 3					; podermos comparar o proximo digito
					DEC     R4						; sem perder os anteriores
					JMP     LOOP_X

FIM_COMP_X:         SHR     R1, 4					; Devolve aos numeros a
					SHR     R2, 4					; representacao de 12 bits
					MOV     M[SP + 2], R1
					MOV     M[SP + 1], R2
					POP     R2
					POP     R1
					POP     R5
					POP     R4
					POP     R3
					RET

;-----------------------------------------------------------------------------
;Comparacao de O
;Descricao: Verifica a existencia do digitos iguais na posicao errada
;-----------------------------------------------------------------------------

COMP_O:             PUSH    R3
					PUSH    R4
					PUSH    R5
					PUSH    R6
					PUSH    R1						; Sequencia gerada resultante de COMP_X   
					PUSH    R2						; sequencia inserida resultante de COMP_X
					
					MOV     R4, 4
LOOP_O1:            CMP     R4, R0
					JMP.Z   FIM_COMP_O
					MOV     M[SP + 2], R1
					AND     R1, 0007h				; Filtra o ultimo digito de R1
					CMP     R1, R0					; e compara com 0, se for 0 é porque
					BR.Z    NEXT_O1 				; o numero ja foi removido em COMP_X
					MOV     R6, 4					; e avanca para o proximo

LOOP_O2:            CMP     R6, R0
					BR.Z    BEF_NEXT_O1
					MOV     M[SP + 1], R2
					AND     R2, 0007h				; Filtra o ultimo digito de R2
					CMP     R1, R2					; para compara-lo com R1
					BR.Z    SAO_O
					BR      NEXT_O2

BEF_NEXT_O1:        SHR     R2, 4					; Devolve a R2 a representacao de 12 bits

NEXT_O1:            DEC     R4
					MOV     R1, M[SP + 2]
					ROR     R1, 3					; Avanca 3 bits em R1
					BR      LOOP_O1					; para comparar o proximo digito

NEXT_O2:            DEC     R6
					MOV     R2, M[SP + 1]
					ROR     R2, 3					; Avanca 3 bits em R2
					BR      LOOP_O2					; para comparar o proximo digito

SAO_O:              MOV     R3, M[REP_INT]
					SHL     R3,3					; Insere o numero 2 na
					ADD     R3, 2					; representacao interna
					MOV     M[REP_INT], R3
					MOV     R5, M[COUNTER_XO]
					INC     R5
					MOV     M[COUNTER_XO], R5
					BR      NEXT_O2

FIM_COMP_O:         POP     R2
					POP     R1
					POP     R6
					POP     R5
					POP     R4
					POP     R3
					RET

;-----------------------------------------------------------------------------
;Poximo Caracter
;Descricao: Avanca o cursor para a proxima coluna
;-----------------------------------------------------------------------------

NEXT_CARACT:        PUSH    R1
					MOV     R1, M[ATIVA_JT]
					INC     R1
					MOV     M[ATIVA_JT], R1
					MOV     M[JT_CONTROL], R1
					POP     R1
					RET

;-----------------------------------------------------------------------------
;Proxima Linha
;Descricao: Avanca o cursor para a proxima linha
;-----------------------------------------------------------------------------

NEXT_LINHA:         PUSH    R1
					MOV     R1, M[ATIVA_JT]
					AND     R1, FF00h				; Filtra o valor das colunas para
					ADD     R1, 0100h				; iniciar o cursos no inicio da linha
					MOV     M[ATIVA_JT], R1
					MOV     M[JT_CONTROL], R1
					POP     R1
					RET

;-----------------------------------------------------------------------------
;Limpa Janela de Texto
;Descricao: Limpa a janela de texto
;-----------------------------------------------------------------------------

LIMPA_JANELA:       PUSH    R1
					PUSH    R2
					PUSH    R3
					PUSH    R4
					MOV     R1, R0
					MOV     M[ATIVA_JT], R1
					MOV     M[JT_CONTROL], R1
					MOV     R2, 15					; Adiciona 79 espacos em cada
LINHAS:             MOV     R3, 79					; linha durante 15 linhas
					CMP     R2, R0
					BR.Z    FIM_LJ
COLUNAS:            CMP     R3, R0
					BR.Z    NEXT_LJ
					MOV     R4, ' '
					MOV     M[JT_WRITE], R4
					CALL 	NEXT_CARACT
					DEC     R3
					BR      COLUNAS
NEXT_LJ:            CALL    NEXT_LINHA
					DEC     R2
					BR      LINHAS

					
FIM_LJ:             MOV     M[ATIVA_JT], R0			; Move o cursor de novo para o
					MOV 	M[JT_CONTROL], R0		; inicio da janela de texto
					POP     R4
					POP     R3
					POP     R2
					POP     R1
					RET

;-----------------------------------------------------------------------------
;Output
;Descricao: Representa a jogada na janela de texto
;-----------------------------------------------------------------------------

OUTPUT:             PUSH    R1
					PUSH    R2
					PUSH    R3
					PUSH    R4
					PUSH    R5
					PUSH    R6
					PUSH    R7

					MOV     R5, M[COUNTER_XO]		; Insere zeros na representacao interna
OUTPUT_AUX:         CMP     R5, 4					; enquanto o COUNTER_XO nao tem o valor 4
					BR.Z    ESCREVE_NUM_TENT
					MOV     R3, M[REP_INT]
					SHL     R3, 3
					MOV     M[REP_INT], R3
					INC     R5
					BR      OUTPUT_AUX

ESCREVE_NUM_TENT:   MOV     R4, M[NUM_TENT]
					CMP     R4, 10
					BR.NN   DOIS_DIGITOS
					ADD     R4, 48					; Adicionamos ao numero que pretendemos
					MOV     M[JT_WRITE], R4			; representar o codigo ASCII do zero
					CALL    NEXT_CARACT
					MOV     R1, ' '
					MOV     M[JT_WRITE], R1
					CALL    NEXT_CARACT
					MOV     M[JT_WRITE], R1
					CALL    NEXT_CARACT
					BR      ESCREVE_NUM_INS

DOIS_DIGITOS:       MOV     R1, 49
					MOV     M[JT_WRITE], R1
					CALL    NEXT_CARACT
					ADD     R4, 38					; Adicionamos ao numero que pretendemos
					MOV     M[JT_WRITE], R4			; representar o codigo ASCII do zero
					CALL    NEXT_CARACT				; menos 10 para obter apenas o 
					MOV     R1, ' '					; ultimo digito
					MOV     M[JT_WRITE], R1
					CALL    NEXT_CARACT
					
ESCREVE_NUM_INS:    MOV     R2, M[NUM_INS]
					MOV     R7, R0					; Como queremos comecar pelo primero digito
					SHL     R2, 4					; deslocamos todos os digitos para eaquerda
CICLO_NUM_INS:      ROL     R2, 3					; Este loop segue sempre a mesma
					PUSH    R2						; logica, em que filtra o primeiro 
					AND     R2, 0007h				; digito do numero e lhe adiciona 
					ADD     R2, 48					; o codigo ASCII do zero
					MOV     M[JT_WRITE], R2
					CALL    NEXT_CARACT
					MOV     R1, 45					; Acrescenta um hifen entre cada numero
					MOV     M[JT_WRITE], R1
					CALL    NEXT_CARACT
					POP     R2
					INC     R7
					CMP     R7, 3
					BR.N    CICLO_NUM_INS
					ROL     R2, 3					; O quarto digito fica fora do loop
					AND     R2, 0007h				; porque depois dele nao queremos um hifen
					ADD     R2, 48
					MOV     M[JT_WRITE], R2
					CALL    NEXT_CARACT

					MOV     R1, ' '
					MOV     M[JT_WRITE], R1
					CALL    NEXT_CARACT
					
					MOV     R7,4
					MOV     R3, M[REP_INT]			; Como queremos comecar pelo primero digito
					SHL     R3, 4					; deslocamos todos os digitos para eaquerda
REP_EXTERNA:        CMP     R7, R0
					JMP.Z   MUDA_LINHA
					ROL     R3, 3
					PUSH    R3
					AND     R3, 0007h				; Este loop segue a mesma logica que o anterior
					CMP     R3, 1					; mas em vez de adicionar o codigo ASCII de zero 
					BR.Z    ESCREVE_X				; compara os numeros com 0, 1 e 2
					CMP     R3, 2					; Se o numero for 1 escreve-se um X
					BR.Z    ESCREVE_O				; se for 2 escreve-se um O e
					MOV     R2, 45					; se for 0 escreve-se um hifen
					MOV     M[JT_WRITE], R2
					CALL    NEXT_CARACT
					DEC     R7
					POP     R3
					JMP     REP_EXTERNA
					
ESCREVE_O:          MOV     R2, 'O'
					MOV     M[JT_WRITE], R2
					CALL    NEXT_CARACT
					DEC     R7
					POP     R3  
					JMP     REP_EXTERNA
					
ESCREVE_X:          MOV     R2, 'X'
					MOV     M[JT_WRITE], R2
					CALL    NEXT_CARACT
					DEC     R7
					POP     R3
					JMP     REP_EXTERNA
					
MUDA_LINHA:         CALL    NEXT_LINHA

FIM_REP_EXTERNA:    POP     R7
					POP     R6
					POP     R5
					POP     R4
					POP     R3
					POP     R2
					POP     R1
					RET

;-----------------------------------------------------------------------------
;Melhor pontuacao
;Descricao: Escreve no LCD a melhor pontuacao obtida
;-----------------------------------------------------------------------------

MELHOR_TENTATIVA:   PUSH    R1
					PUSH    R2
					PUSH    R3
					MOV     R1, ATIVA_LCD
					MOV     M[LCD_CONTROL], R1

					MOV     R3, M[CARACT_FINAL]
					MOV     R2, MSG_DISPLAY
MSG_RECORDE:        MOV     R4, M[R2]
					CMP     R3, M[R2]
					BR.Z    RECORDE
					MOV     M[LCD_WRITE], R4
					INC     R2
					INC     R1
					MOV     M[LCD_CONTROL], R1
					BR      MSG_RECORDE

RECORDE:            MOV     R2, M[NUM_TENT]			; Decrementamos o valor de R2 porque
					DEC     R2						; antes desta rotina ser chamada
					MOV     R3, M[MELHOR_TENT]		; este valor tinha sido incrementado
					CMP     R2, R3					
					JMP.NN  FIM_MT					; Se a ultima jogada for menor que a nelhor
					MOV     M[MELHOR_TENT], R2		; tentativa o LCD e atualizado
					MOV     R3, M[MELHOR_TENT]
					CMP     R3, 10
					BR.NN   LCD_2_DIG

LCD_1_DIG:          ADD     R3, 48
					MOV     M[LCD_WRITE], R3
					INC     R1
					MOV     M[LCD_CONTROL], R1
					MOV     R2, 32
					MOV     M[LCD_WRITE], R2
					BR      FIM_MT

LCD_2_DIG:          MOV     R2, 49
					MOV     M[LCD_WRITE], R2
					INC     R1
					MOV     M[LCD_CONTROL], R1
					SUB     R3, 10
					ADD     R3, 48
					MOV     M[LCD_WRITE], R3                   

FIM_MT:             POP     R3
					POP     R2
					POP     R1
					RET

;#############################################################################
;	CODIGO PRINCIPAL
;#############################################################################

INICIO:             MOV     R1, SP_INICIAL
					MOV     SP, R1
					MOV     R7, ATIVA_INT
					MOV     M[INT_CONTROL], R7
					MOV     R1, FFFFh
					MOV     M[JT_CONTROL], R1

					MOV     R1, MSG_INICIAL
RESTART:            CALL    NOVO_JOGO

CONT_TENT:          MOV     R1, 1
					MOV     M[NUM_TENT], R1
					MOV     R1, R0
					MOV     M[JT_CONTROL], R1
					MOV     M[ATIVA_JT], R1
					
CICLO:              CALL    DISPLAY
					MOV     R4, M[NUM_TENT]    
					CMP     R4, M[MAX_TENT]
					JMP.P   MSG_F
					CALL    NUM_INSERIDO

INVOCA_CX:          CALL    COMP_X

INVOCA_CO:          CALL    COMP_O

INVOCA_OUTPUT:      CALL    OUTPUT

NOVA_JOGADA:        MOV     R4, M[NUM_TENT]
					INC     R4
					MOV     M[NUM_TENT], R4
					MOV     R3, M[REP_INT]			; Para verificar se o jogador venceu
					AND     R3, 0FFFh				; comparamos se REP_INT e igual a
					CMP     R3, 0000001001001001b	; 0000001001001001b que representa o
					BR.Z    INVOCA_MT				; o numero 1111 representado em 12 bits
					JMP     CICLO

INVOCA_MT:          CALL    MELHOR_TENTATIVA

MSG_F:              MOV     R1, MSG_FINAL_1
					MOV     R3, M[CARACT_FINAL]
BACK_2:             MOV     R2, M[R1]
					CMP     R3, R2
					BR.Z    NEXT_MSG
					MOV     M[JT_WRITE], R2
					CALL    NEXT_CARACT
					INC     R1
					BR      BACK_2
					
NEXT_MSG:           CALL    NEXT_LINHA
					MOV     R1, MSG_FINAL_2			; Altera a mensagem inicial 
													; para a mensagem de restart
GAME_OVER:          JMP     RESTART           

;-----------------------------------------------------------------------------

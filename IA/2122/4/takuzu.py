# takuzu.py: Template para implementação do projeto de Inteligência Artificial 2021/2022.
# Devem alterar as classes e funções neste ficheiro de acordo com as instruções do enunciado.
# Além das funções e classes já definidas, podem acrescentar outras que considerem pertinentes.

# Grupo 00:
# 00000 Nome1
# 00000 Nome2

from sys import stdin
from search import (
	Problem,
	Node,
	breadth_first_tree_search,
	depth_first_tree_search,
	greedy_search,
	astar_search,
	compare_searchers
)
import numpy as np


class TakuzuState:
	state_id = 0

	def __init__(self, board):
		self.board = board
		self.id = TakuzuState.state_id
		TakuzuState.state_id += 1

	def __lt__(self, other):
		return self.id < other.id


class Board:
	"""Representação interna de um tabuleiro de Takuzu."""

	def __init__(self, data):
		self.data = data
		self.size = len(data)
		if (self.size % 2 == 0):
			self.max = int(self.size / 2)
		else:
			self.max = int((self.size / 2) + 1)

	def __str__(self):
		""""Representação externa de um tabuleiro de Takuzu."""
		string = ""
		for row in self.data:
			string += '\t'.join([str(n) for n in row]) + '\n'
		return string

	def get_number(self, row: int, col: int) -> int:
		"""Devolve o valor na respetiva posição do tabuleiro."""
		if row < 0 or col < 0:
			return None
		else:
			try:
				return self.data[row][col]
			except IndexError:
				return None

	def adjacent_vertical_numbers(self, row: int, col: int) -> (int, int):
		"""Devolve os valores imediatamente abaixo e acima,
		respectivamente."""
		return (self.get_number(row+1, col), self.get_number(row-1, col))

	def adjacent_horizontal_numbers(self, row: int, col: int) -> (int, int):
		"""Devolve os valores imediatamente à esquerda e à direita,
		respectivamente."""
		return (self.get_number(row, col-1), self.get_number(row, col+1))

	@staticmethod
	def parse_instance_from_stdin():
		"""Lê o test do standard input (stdin) que é passado como argumento
		e retorna uma instância da classe Board."""
		n = stdin.readline()
		board = []
		for line in stdin:
			row = []
			for i in range(int(n)):
				row += [int(line[i])]
				line = line[1:]
			board += [row]
		return Board(np.array(board))

	def transpose(self):
		return Board(self.data.transpose())


class Takuzu(Problem):
	def __init__(self, board: Board):
		"""O construtor especifica o estado inicial."""
		super().__init__(TakuzuState(board))

	def actions_aux(self, board: Board):
		actions = []
		for row in range(board.size):
			twos = np.where(board.data[row] == 2)[0]	# Obtém as posições vazias
			if len(twos) == 0:			# Ignora a linha caso nao existam
				continue 
			elif len(twos) <= 2:		# Verifica se a linha é parcialmente repetida
				fixed = False
				test_row = np.delete(board.data[row].copy(), twos)	# Faz uma cópia da linha a
				for i in range(board.size):							# verificar sem as posições vazias
					if i != row and np.count_nonzero(board.data[i] == 2) == 0:  # Faz uma copia da outra
						test_i = np.delete(board.data[i].copy(), twos)			# linha, caso esta esteja
						if np.array_equal(test_row, test_i):					# completamente preenchida, 
							for col in twos:									# e verifica se são iguais
								if np.array_equal(board.data[i][col], 0):
									actions += [(row, col, 1)]
								elif np.array_equal(board.data[i][col], 1):
									actions += [(row, col, 0)]
							fixed = True
							break
				if fixed:
					continue
			ones = np.count_nonzero(board.data[row] == 1)
			zeros = np.count_nonzero(board.data[row] == 0)
			for col in twos:			# Verifica apenas as posições vazias
				# Verifica os numeros máximos de zeros e uns
				if zeros == board.max:
					actions += [(row, col, 1)]
					continue
				elif ones == board.max:
					actions += [(row, col, 0)]
					continue
				# Verifica que não exitem mais de 2 números iguais seguidos
				ll = board.get_number(row, col-2)
				l, r = board.adjacent_horizontal_numbers(row, col)
				rr = board.get_number(row, col+2)
				if l == ll and l != None and l != 2:
					if l == 0:
						actions += [(row, col, 1)]
					elif l == 1:
						actions += [(row, col, 0)]
				elif r == rr and r != None and r != 2:
					if r == 0:
						actions += [(row, col, 1)]
					elif r == 1:
						actions += [(row, col, 0)]
				elif l == r:
					if r == 0:
						actions += [(row, col, 1)]
					elif r == 1:
						actions += [(row, col, 0)]
		return actions

	def actions(self, state: TakuzuState):
		"""Retorna uma lista de ações que podem ser executadas a
		partir do estado passado como argumento."""
		board = state.board
		actions = self.actions_aux(board)			# Verifica as linhas
		t_board = board.transpose()					# Transpõe o tabuleiro
		t_actions = self.actions_aux(t_board)		# Verifica as colunas
		for action in t_actions:				# Transpõe as ações
			col, row, number = action			# obtidas ao verificar
			actions += [(row, col, number)]		# as colunas
		return actions

	def result(self, state: TakuzuState, action):
		"""Retorna o estado resultante de executar a 'action' sobre
		'state' passado como argumento. A ação a executar deve ser uma
		das presentes na lista obtida pela execução de
		self.actions(state)."""
		row, col, number = action
		state.board.data[row][col] = number
		return TakuzuState(Board(state.board.data))

	def goal_test(self, state: TakuzuState):
		"""Retorna True se e só se o estado passado como argumento é
		um estado objetivo. Deve verificar se todas as posições do tabuleiro
		estão preenchidas com uma sequência de números adjacentes."""
		board = state.board.data
		if np.count_nonzero(board == 2) > 0:
			return False

		t_board = board.transpose()

		for i in range(state.board.size):
			if (np.count_nonzero(board[i] == 0) > state.board.max or
				np.count_nonzero(board[i] == 1) > state.board.max or
				np.count_nonzero(t_board[i] == 0) > state.board.max or
				np.count_nonzero(t_board[i] == 1) > state.board.max):
				return False
			
			r_element = -1
			r_count = 0
			c_element = -1
			c_count = 0
			for j in range(state.board.size):
				if board[i][j] != r_element:
					r_element = board[i][j]
					r_count = 0
				elif board[i][j] == r_element:
					r_count += 1
				
				if t_board[i][j] != c_element:
					c_element = t_board[i][j]
					c_count = 0
				elif t_board[i][j] == c_element:
					c_count += 1

				if r_count > 2 or c_count > 2:
					return False

				if j != i:
					if (np.array_equal(board[i], board[j]) or
						np.array_equal(t_board[i], t_board[j])):
						return False

		return True 

	def h(self, node: Node):
		"""Função heuristica utilizada para a procura A*."""
		zeros = np.count_nonzero(node.state.board.data == 0)
		ones = np.count_nonzero(node.state.board.data == 1)
		twos = np.count_nonzero(node.state.board.data == 2)
		return abs(zeros - ones) + twos


if __name__ == "__main__":
	board = Board.parse_instance_from_stdin()
	problem = Takuzu(board)
	#'''
	compare_searchers([problem], ['Searcher', 'Successors | Goal_Tests | States | Found'],
					   searchers=[breadth_first_tree_search,
                                  depth_first_tree_search,
                                  greedy_search,
								  astar_search])
	#'''
	goal_node = astar_search(problem)
	print(goal_node.state.board, end='')
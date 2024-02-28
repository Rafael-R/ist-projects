# bimaru.py: Projeto de Inteligência Artificial 2022/2023.

# Grupo 103:
# 89532 Rafael Sousa Rodrigues

import copy
from sys import stdin
from typing import Tuple, Optional
from search import (
    Problem,
    Node,
    depth_first_tree_search,
)


# Símbolos correspondentes a cada valor
values = {
    "undefined": ["u"],
    "middle": ["m", "M"],
    "circle": ["c", "C"],
    "top": ["t", "T"],
    "bottom": ["b", "B"],
    "left": ["l", "L"],
    "right": ["r", "R"],
    "water": ["w", "W"],
}

# Posições adjacentes a preencher com água para cada valor
surrounding = {
    "undefined": [[-1, -1], [-1, 1], [1, 1], [1, -1]],
    "circle": [[0, -1], [-1, -1], [-1, 0], [-1, 1], [0, 1], [1, 1], [1, 0], [1, -1]],
    "top": [[1, -1], [0, -1], [-1, -1], [-1, 0], [-1, 1], [0, 1], [1, 1]],
    "bottom": [[-1, 1], [0, 1], [1, 1], [1, 0], [1, -1], [0, -1], [-1, -1]],
    "left": [[1, 1], [1, 0], [1, -1], [0, -1], [-1, -1], [-1, 0], [-1, 1]],
    "right": [[-1, -1], [-1, 0], [-1, 1], [0, 1], [1, 1], [1, 0], [1, -1]],
    "middle": [[-1, -1], [-1, 1], [1, 1], [1, -1]],
}

# Sequências de símbolos correspondentes a um barco, dependendo da orientação e do tamanho
boats = {
    "horizontal": {2: ["l", "r"], 3: ["l", "m", "r"], 4: ["l", "m", "m", "r"]},
    "vertical": {2: ["t", "b"], 3: ["t", "m", "b"], 4: ["t", "m", "m", "b"]},
}


class BimaruState:
    state_id = 0

    def __init__(self, board):
        self.board = board
        self.id = BimaruState.state_id
        BimaruState.state_id += 1

    def __lt__(self, other):
        return self.id < other.id


class Board:
    """Representação interna de um tabuleiro de Bimaru."""

    def __init__(self, matrix, rows, cols):
        # matriz que representa o tabuleiro
        self.matrix = matrix
        # posições por preencher com partes de barco
        self.rows_left = rows
        self.cols_left = cols
        # posições vazias
        self.rows_empty = [10 for _ in range(10)]
        self.cols_empty = [10 for _ in range(10)]
        # barcos já descobertos
        self.boats = {1: [], 2: [], 3: [], 4: []}
        # posições de pistas
        self.hints = []

    def __str__(self):
        s = ""
        for row in range(10):
            for col in range(10):
                value = self.matrix[row][col]
                if value == "w":
                    s += "."
                else:
                    s += value
            s += "\n"
        return s

    def get_value(self, row: int, col: int) -> Optional[str]:
        """Retorna o valor na respetiva posição do tabuleiro."""
        if row < 0 or col < 0 or row > 9 or col > 9:
            return None
        else:
            return self.matrix[row][col]

    def set_value(self, row: int, col: int, value: str, check: bool = True) -> None:
        """Insere o valor na respetiva posição do tabuleiro e
        preenche as devidas posições adjacentes com água."""
        if value == "W" and self.matrix[row][col] == "w":
            self.matrix[row][col] = value

        if self.matrix[row][col] != " ":
            return

        self.matrix[row][col] = value
        self.rows_empty[row] -= 1
        self.cols_empty[col] -= 1

        if value in values["water"]:
            return

        self.rows_left[row] -= 1
        self.cols_left[col] -= 1
        self.fill_surrounding(row, col, value)

        if check:
            self.check_boat(row, col, value)

    def del_value(self, row: int, col: int):
        """Apaga um valor inserido anteriormente."""
        self.matrix[row][col] = " "
        self.rows_empty[row] += 1
        self.cols_empty[col] += 1
        self.rows_left[row] += 1
        self.cols_left[col] += 1

    def adjacent_vertical_values(
        self, row: int, col: int
    ) -> Tuple[Optional[str], Optional[str]]:
        """Retorna os valores imediatamente acima e abaixo,
        respetivamente."""
        return (self.get_value(row - 1, col), self.get_value(row + 1, col))

    def adjacent_horizontal_values(
        self, row: int, col: int
    ) -> Tuple[Optional[str], Optional[str]]:
        """Retorna os valores imediatamente à esquerda e à direita,
        respetivamente."""
        return (self.get_value(row, col - 1), self.get_value(row, col + 1))

    def adjacent_values(
        self, row: int, col: int
    ) -> Tuple[Optional[str], Optional[str], Optional[str], Optional[str]]:
        """Retorna os valores imediatamente à esquerda, em cima, à direita
        e em baixo, respetivamente."""
        t, b = self.adjacent_vertical_values(row, col)
        l, r = self.adjacent_horizontal_values(row, col)
        return (l, t, r, b)

    def fill_water(self) -> bool:
        """Preenche com água linhas e colunas sem posições livres
        para partes de barco.\n
        Retorna True caso tenham sido feitas alterações."""
        changed = False
        for i in range(10):
            if self.rows_left[i] == 0 and self.rows_empty[i] > 0:
                changed = True
                for j in range(10):
                    self.set_value(i, j, "w")
            if self.cols_left[i] == 0 and self.cols_empty[i] > 0:
                changed = True
                for j in range(10):
                    self.set_value(j, i, "w")
        return changed

    def fill_surrounding(self, row: int, col: int, value: str) -> None:
        """Preenche com água as posições adjacentes a uma dada posição,
        tendo em conta o seu valor."""
        positions = [surrounding[v] for v in values if value in values[v]][0]
        for r, c in positions:
            r += row
            c += col
            if r >= 0 and c >= 0 and r <= 9 and c <= 9:
                self.set_value(r, c, "w")

    def fill_parts(self) -> list:
        """Preenche com partes de barco indefinidas linhas e colunas cujo número 
        de partes em falta corresponde ao número de posições livres. \n
        Retorna uma lista com estas posições."""
        positions = []
        for i in range(10):
            if self.rows_left[i] == self.rows_empty[i] != 0:
                for j in range(10):
                    if self.matrix[i][j] == " ":
                        self.set_value(i, j, "u")
                        positions += [(i, j)]
            if self.cols_left[i] == self.cols_empty[i] != 0:
                for j in range(10):
                    if self.matrix[j][i] == " ":
                        self.set_value(j, i, "u")
                        positions += [(j, i)]
        return positions

    def fill_boat(self, positions: list) -> None:
        """Recebe uma lista de posições correspondente a um barco incompleto.
        Preenche as posições vazias com os valores corretos."""
        size = len(positions)
        orientation = Board.get_orientation(positions)
        for i in range(len(positions)):
            row, col = positions[i]
            if self.matrix[row][col] == " ":
                self.set_value(row, col, boats[orientation][size][i], False)

    def get_possible_boat_positions(self, size: int) -> list:
        """Retorna uma lista de possíveis posições para um barco
        de um dado tamanho."""
        lines = []
        for i in range(10):
            if self.rows_left[i] >= size:
                line = []
                for j in range(10):
                    if self.matrix[i][j] == " ":
                        line += [(i, j)]
                    elif self.matrix[i][j] != " " or self.cols_left[j] == 0:
                        lines += [line]
                        line = []
                lines += [line]
            if self.cols_left[i] >= size:
                line = []
                for j in range(10):
                    if self.matrix[j][i] == " ":
                        line += [(j, i)]
                    elif self.matrix[i][j] != " " or self.rows_left[j] == 0:
                        lines += [line]
                        line = []
                lines += [line]
        lines = [line for line in lines if len(line) >= size]
        possibilities = []
        for line in lines:
            orientation = Board.get_orientation(line)
            iters = len(line) - (size - 1)
            for i in range(iters):
                pos = []
                for j in range(size):
                    row, col = line[i : size + i][j]
                    if j == 0:
                        if orientation == "horizontal":
                            pos += [(row, col, "l")]
                        elif orientation == "vertical":
                            pos += [(row, col, "t")]
                    elif j == size - 1:
                        if orientation == "horizontal":
                            pos += [(row, col, "r")]
                        elif orientation == "vertical":
                            pos += [(row, col, "b")]
                    else:
                        pos += [(row, col, "m")]
                possibilities += [pos]
        return possibilities

    def check_boat(self, row: int, col: int, value: str) -> None:
        """Dada uma posição preenchida com uma parte de um barco,
        verifica recorrendo às posições adjacentes se forma um barco."""
        boat = None
        if value in values["circle"]:
            boat = [(row, col)]
        elif value in values["middle"]:
            t, l, r, b = self.adjacent_values(row, col)
            if t in values["top"] and b in values["bottom"]:
                boat = [(row - 1, col), (row, col), (row + 1, col)]
            elif l in values["left"] and r in values["right"]:
                boat = [(row, col - 1), (row, col), (row, col + 1)]
            elif t in values["top"] and b in values["middle"]:
                boat = [(row - 1, col), (row, col), (row + 1, col), (row + 2, col)]
            elif t in values["middle"] and b in values["bottom"]:
                boat = [(row - 2, col), (row - 1, col), (row, col), (row + 1, col)]
            elif l in values["left"] and r in values["middle"]:
                boat = [(row, col - 1), (row, col), (row, col + 1), (row, col + 2)]
            elif l in values["middle"] and r in values["right"]:
                boat = [(row, col - 2), (row, col - 1), (row, col), (row, col + 1)]
        else:
            attempt = [(row, col)]
            for i in range(1, 4):
                if value in values["top"]:
                    attempt = attempt + [(row + i, col)]
                    if self.get_value(row + i, col) in values["bottom"]:
                        boat = attempt
                        break
                elif value in values["bottom"]:
                    attempt = [(row - i, col)] + attempt
                    if self.get_value(row - i, col) in values["top"]:
                        boat = attempt
                        break
                elif value in values["left"]:
                    attempt = attempt + [(row, col + i)]
                    if self.get_value(row, col + i) in values["right"]:
                        boat = attempt
                        break
                elif value in values["right"]:
                    attempt = [(row, col - i)] + attempt
                    if self.get_value(row, col - i) in values["left"]:
                        boat = attempt
                        break
        # Caso encontre algum barco
        if boat != None:
            # Remove as posições pertencentes a um barco da lista de pistas
            self.hints = list(set(self.hints) - set(boat))
            if len(boat) > 2:
                self.fill_boat(boat)
            # Adiciona o barco à lista de barcos encontrados
            self.boats[len(boat)] += [boat]

    def is_isolated(self, row: int, col: int) -> bool:
        """Verifica se as adjacências de uma dada posição estão vazias."""
        l, t, r, b = self.adjacent_values(row, col)
        if l == " " and t == " " and r == " " and b == " ":
            return True
        return False

    def guess(self, row: int, col: int) -> list:
        """Tenta descobrir o valor das posições indefinidas."""
        l, t, r, b = self.adjacent_values(row, col)
        if t in values["middle"] + values["top"] + values["undefined"]:
            if b in values["water"] + [None]:
                return [(row, col, "b")]
            elif b in values["middle"] + values["bottom"] + values["undefined"]:
                return [(row, col, "m")]
            else:
                if (
                    t in values["top"]
                    and len(self.boats[3]) == 2
                    and len(self.boats[4]) == 1
                ):
                    return [(row, col, "b")]
                elif (
                    b in values["middle"]
                    and self.get_value(row, col - 2) in values["top"]
                    and len(self.boats[4]) == 1
                ):
                    return [(row, col, "b")]
                return [(row, col, "b"), (row, col, "m")]
        elif b in values["middle"] + values["bottom"] + values["undefined"]:
            if t in values["water"] + [None]:
                return [(row, col, "t")]
            elif t in values["middle"] + values["top"] + values["undefined"]:
                return [(row, col, "m")]
            else:
                if (
                    b in values["bottom"]
                    and len(self.boats[3]) == 2
                    and len(self.boats[4]) == 1
                ):
                    return [(row, col, "t")]
                elif (
                    b in values["middle"]
                    and self.get_value(row, col + 2) in values["bottom"]
                    and len(self.boats[4]) == 1
                ):
                    return [(row, col, "t")]
                return [(row, col, "t"), (row, col, "m")]
        elif l in values["middle"] + values["left"] + values["undefined"]:
            if r in values["water"] + [None]:
                return [(row, col, "r")]
            elif r in values["middle"] + values["right"] + values["undefined"]:
                return [(row, col, "m")]
            else:
                if (
                    l in values["left"]
                    and len(self.boats[3]) == 2
                    and len(self.boats[4]) == 1
                ):
                    return [(row, col, "r")]
                elif (
                    l in values["middle"]
                    and self.get_value(row - 2, col) in values["left"]
                    and len(self.boats[4]) == 1
                ):
                    return [(row, col, "r")]
                return [(row, col, "r"), (row, col, "m")]
        elif r in values["middle"] + values["right"] + values["undefined"]:
            if l in values["water"] + [None]:
                return [(row, col, "l")]
            elif l in values["middle"] + values["left"] + values["undefined"]:
                return [(row, col, "m")]
            else:
                if (
                    r in values["right"]
                    and len(self.boats[3]) == 2
                    and len(self.boats[4]) == 1
                ):
                    return [(row, col, "l")]
                elif (
                    r in values["middle"]
                    and self.get_value(row + 2, col) in values["right"]
                    and len(self.boats[4]) == 1
                ):
                    return [(row, col, "l")]
                return [(row, col, "l"), (row, col, "m")]
        if (
            len(self.boats[4]) == 1
            and len(self.boats[3]) == 2
            and len(self.boats[2]) == 3
        ):
            return [(row, col, "c")]
        elif (
            t in values["water"] + [None]
            and b in values["water"] + [None]
            and l in values["water"] + [None]
            and r in values["water"] + [None]
            and len(self.boats[1]) < 4
        ):
            return [(row, col, "c")]
        return []

    def process(self):
        """Preenche recursivamente, e se possível, as posições vazias do
        tabuleiro com água e com partes de barco, até não haverem alterações"""
        changed = self.fill_water()
        undefined = self.fill_parts()
        for row, col in undefined:
            guess = self.guess(row, col)
            self.del_value(row, col)
            if len(guess) == 1:
                changed = True
                self.hints += [(row, col)]
                self.set_value(row, col, guess[0][2])
        if changed:
            self.process()

    @staticmethod
    def get_orientation(positions: list) -> str:
        """ "Dada uma lista de posições, retorna a sua orientação."""
        if positions[0][0] == positions[1][0]:
            return "horizontal"
        else:
            return "vertical"

    @staticmethod
    def parse_instance():
        """Lê o test do standard input (stdin) que é passado como 
        argumento e retorna uma instância da classe Board."""
        input = [line.split() for line in stdin]
        matrix = [[" " for _ in range(10)] for _ in range(10)]
        rows = [int(i) for i in input[0][1:]]
        cols = [int(i) for i in input[1][1:]]
        board = Board(matrix, rows, cols)
        for _, row, col, value in input[3 : int(input[2][0]) + 3]:
            row = int(row)
            col = int(col)
            if value not in values["water"]:
                board.hints += [(row, col)]
            board.set_value(row, col, value)
        board.process()
        return board


class Bimaru(Problem):
    def __init__(self, board: Board):
        """O construtor especifica o estado inicial."""
        super().__init__(BimaruState(board))

    def actions(self, state: BimaruState):
        """Retorna uma lista de ações que podem ser executadas a
        partir do estado passado como argumento."""
        scratch = copy.deepcopy(state.board)

        undefined = []
        possibilities = []
        for row, col in scratch.hints:
            value = scratch.matrix[row][col]
            l, t, r, b = scratch.adjacent_values(row, col)
            if value in values["middle"]:
                if l in values["water"] + [None] or r in values["water"] + [None]:
                    if t == " ":
                        scratch.set_value(row - 1, col, "u")
                        undefined += [(row - 1, col)]
                    if b == " ":
                        scratch.set_value(row + 1, col, "u")
                        undefined += [(row + 1, col)]
                elif t in values["water"] + [None] or b in values["water"] + [None]:
                    if l == " ":
                        scratch.set_value(row, col - 1, "u")
                        undefined += [(row, col - 1)]
                    if r == " ":
                        scratch.set_value(row, col + 1, "u")
                        undefined += [(row, col + 1)]
                # Caso em que M não tem posições adjacentes preenchidas
                elif scratch.is_isolated(row, col):
                    possibilities += [
                        [
                            (row - 1, col, "w"),
                            (row + 1, col, "w"),
                        ],  # M pertence a barco horizontal
                        [
                            (row, col - 1, "w"),
                            (row, col + 1, "w"),
                        ],  # M pertence a barco vertical
                    ]
            elif value in values["left"] and r == " ":
                scratch.set_value(row, col + 1, "u")
                undefined += [(row, col + 1)]
            elif value in values["top"] and b == " ":
                scratch.set_value(row + 1, col, "u")
                undefined += [(row + 1, col)]
            elif value in values["right"] and l == " ":
                scratch.set_value(row, col - 1, "u")
                undefined += [(row, col - 1)]
            elif value in values["bottom"] and t == " ":
                scratch.set_value(row - 1, col, "u")
                undefined += [(row - 1, col)]

        while True:
            scratch.fill_water()
            new_undefined = scratch.fill_parts()
            undefined += new_undefined
            if new_undefined == []:
                break

        correct = []
        for row, col in undefined:
            guess = scratch.guess(row, col)
            # Apaga a posição indefinida pois esse espaço vazio poderá ser usado de seguida
            if len(guess) == 0:
                scratch.del_value(row, col)
            if len(guess) == 1:
                correct += guess
            else:
                possibilities += guess

        # Retorna apenas as posições corretas como uma única ação
        if correct != []:
            return [correct]
        # Caso não hajam, retorna uma lista com possibilidades
        elif possibilities != []:
            return possibilities

        # possíveis posições para barcos de tamanho 4
        if len(scratch.boats[4]) < 1:
            return scratch.get_possible_boat_positions(4)
        # possíveis posições para barcos de tamanho 3
        if len(scratch.boats[4]) == 1 and len(scratch.boats[3]) < 2:
            return scratch.get_possible_boat_positions(3)
        # possíveis posições para barcos de tamanho 2
        if len(scratch.boats[3]) == 2 and len(scratch.boats[2]) < 3:
            return scratch.get_possible_boat_positions(2)

        return []

    def result(self, state: BimaruState, action):
        """Retorna o estado resultante de executar a 'action' sobre
        'state' passado como argumento. A ação a executar deve ser uma
        das presentes na lista obtida pela execução de self.actions(state)."""
        board = copy.deepcopy(state.board)
        # Caso receba uma lista de ações
        if isinstance(action, list):
            for row, col, value in action:
                board.hints += [(row, col)]
                board.set_value(row, col, value)
        # Caso receba uma única ação
        else:
            row, col, value = action
            board.hints += [(row, col)]
            board.set_value(row, col, value)
        board.process()
        return BimaruState(board)

    def goal_test(self, state: BimaruState):
        """Retorna True se e só se o estado passado como argumento é
        um estado objetivo. Deve verificar se todas as posições do 
        tabuleiro estão preenchidas de acordo com as regras do problema."""
        board = state.board
        return (
            all(v == 0 for v in board.rows_left)
            and all(v == 0 for v in board.cols_left)
            and all(v == 0 for v in board.rows_empty)
            and all(v == 0 for v in board.cols_empty)
            and len(board.boats[1]) == 4
            and len(board.boats[2]) == 3
            and len(board.boats[3]) == 2
            and len(board.boats[4]) == 1
        )

    def h(self, node: Node):
        """Função heurística utilizada para a procura A*."""
        # Não utilizada devido ao uso de DFS
        pass


if __name__ == "__main__":
    board = Board.parse_instance()
    problem = Bimaru(board)
    goal_node = depth_first_tree_search(problem)

    if goal_node is None:
        print("No solution found")
    else:
        print(goal_node.state.board, end="")

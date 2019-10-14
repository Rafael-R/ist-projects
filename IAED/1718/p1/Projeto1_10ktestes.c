/* ---------------------------------------------------------------------------------- */
/*                       Nome: Rafael Rodrigues - Número: 89532                       */
/*                                                                                    */
/*                                     1º Projeto                                     */
/*                  Introdução aos Algoritmos e Estruturas de Dados                   */
/* ---------------------------------------------------------------------------------- */

/* ----------------------------------- BIBLIOTECAS ----------------------------------- */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

/* ----------------------------------- CONSTANTES ----------------------------------- */

#define MAX_ELEMENTS 10000
#define MAX_FILENAME 81
#define MAX_OFFSET 20000

/* -------------------------------- VARIÁVEIS GLOBAIS -------------------------------- */

unsigned long int mini, minj, maxi, maxj, nelem, size;
double zero, dens;
char parameter[9], temporary_filename[MAX_FILENAME], filename[MAX_FILENAME];
int filename_control;

typedef struct {
    unsigned long int line;
    unsigned long int column;
    double value;
} values;

values elements[MAX_ELEMENTS];

typedef struct {
    unsigned long int elements;
    unsigned long int index;
} lines;

/* ------------------------ DECLARAÇÃO DAS FUNÇÕES PRINCIPAIS ------------------------ */

void add_element(unsigned long int line, unsigned long int column, double value);

void list_elements();

void info_matrix();

void list_line(unsigned long int line);

void list_column(unsigned long int column);

void sort_matrix();

void redefine_zero(double value);

void compress_matrix();

void save_matrix();

/* ------------------------ DECLARAÇÃO DAS FUNÇÕES AUXILIARES ------------------------ */

void calc_mini();

void calc_minj();

void calc_maxi();

void calc_maxj();

void calc_size();

void calc_dens();

void refresh_info();

void sort(values array[MAX_ELEMENTS], int option, unsigned long int size);

/* -------------------------------------- MAIN -------------------------------------- */

int main(int argc, char const *argv[]) {
    char command;
    unsigned long int line, column;
    double value;
    zero = 0; mini = 0; minj = 0; maxi = 0; maxj = 0;
    nelem = 0; size = 0; dens = 0; filename_control = 0;
    if (argc == 2) {
        FILE *read = fopen(argv[1], "r");
        if (read != NULL) {
            strcpy(temporary_filename, argv[1]); filename_control = 1;
            while (3 == fscanf(read, "%lu %lu %lf", &line, &column, &value)) {
                add_element(line, column, value);
            }
            fclose(read);
        }
    }
    while (command != 'q') {
        scanf("%c", &command);
        switch (command) {
            case 'a':
                scanf("%lu %lu %lf", &line, &column, &value);
                add_element(line, column, value); break;
            case 'p':
                list_elements(); break;
            case 'i':
                info_matrix(); break;
            case 'l':
                scanf("%lu", &line);
                list_line(line); break;
            case 'c':
                scanf("%lu", &column);
                list_column(column); break;
            case 'o':
                fgets(parameter, sizeof parameter, stdin);
                sort_matrix(); break;
            case 'z':
                scanf("%lf", &value);
                redefine_zero(value); break;
            case 's':
                compress_matrix(); break;
            case 'w':
                fgets(filename, sizeof filename, stdin);
                save_matrix(); break;
        }
    }
    return 0;
}

/* ------------------------ DEFINIÇÃO DAS FUNÇÕES PRINCIPAIS ------------------------ */

void add_element(unsigned long int line, unsigned long int column, double value) {
    int i, j, control = 0;
    refresh_info();
    if (value == zero) {
        for (i = 0; i < nelem; i++) {
            if (elements[i].line == line && elements[i].column == column) {
                for (j = i; j < nelem - 1; j++) {
                    elements[j] = elements[j + 1];
                }
                elements[nelem - 1].value = zero;
                nelem--;
            }
        }
    } else {
        if (nelem == 0) {
            elements[nelem].line = line;
            elements[nelem].column = column;
            elements[nelem].value = value;
            nelem++;
        } else {
            for (i = 0; i < nelem; i++) {
                if (elements[i].line == line && elements[i].column == column) {
                    elements[i].value = value; control = 1;
                }
            }
            if (control == 0) {
                elements[nelem].line = line;
                elements[nelem].column = column;
                elements[nelem].value = value;
                nelem++;
            }
        }
    }
}

void list_elements() {
    int i;
    refresh_info();
    if (nelem == 0) {
        printf("empty matrix\n");
    } else {
        for (i = 0; i < nelem; i++) {
            printf("[%lu;%lu]=%.3lf\n", elements[i].line,
            elements[i].column, elements[i].value);
        }
    }
}

void info_matrix() {
    refresh_info();
    if (nelem == 0) {
        printf("empty matrix\n");
    } else {
        printf("[%lu %lu] [%lu %lu] %lu / %lu = %.3f%%\n",
        mini, minj, maxi, maxj, nelem, size, dens * 100);
    }
}

void list_line(unsigned long int line) {
    int i, counter = 0, control;
    refresh_info();
    for (i = 0; i < nelem; i++) {
        if (elements[i].line == line && elements[i].value != zero) {
            counter++;
        }
    }
    if (counter == 0) {
        printf("empty line\n");
    } else {
        while (minj <= maxj) {
            control = 0;
            for (i = 0; i < nelem; i++) {
                if (elements[i].line == line && elements[i].column == minj && elements[i].value != zero) {
                    printf(" %.3lf", elements[i].value); control = 1;
                }
            }
            if (control == 0) {
                printf(" %.3lf", zero);
            }
            minj++;
        }
        printf("\n");
    }
}

void list_column(unsigned long int column) {
    int j, counter = 0, control;
    refresh_info();
    for (j = 0; j < nelem; j++) {
        if (elements[j].column == column  && elements[j].value != zero) {
            counter++;
        }
    }
    if (counter == 0) {
        printf("empty column\n");
    } else {
        while (mini <= maxi) {
            control = 0;
            for (j = 0; j < nelem; j++) {
                if (elements[j].line == mini && elements[j].column == column && elements[j].value != zero) {
                    printf("[%lu;%lu]=%.3lf\n", elements[j].line, elements[j].column, elements[j].value);
                    control = 1;
                }
            }
            if (control == 0) {
                printf("[%lu;%lu]=%.3lf\n", mini, column, zero);
            }
            mini++;
        }
    }
}

void sort_matrix() {
    char column[7] = "column";
    memmove(parameter, parameter + 1, strlen(parameter));
    parameter[strlen(parameter) - 1] = 0;
    refresh_info();
    if (strlen(parameter) == 0) {
        sort(elements, 1, nelem); sort(elements, 0, nelem);
    } else if (strcmp(parameter, column) == 0) {
        sort(elements, 0, nelem); sort(elements, 1, nelem);
    }
}

void redefine_zero(double value) {
    int i, j, counter = 0;
    refresh_info();
    if (nelem > 0) {
        for (i = 0; i < nelem; i++) {
            if (elements[i].value == value) {
                counter++;
            }
        }
        while (counter > 0) {
            for (i = 0; i < nelem; i++) {
                if (elements[i].value == value) {
                    for (j = i; j < nelem; j++) {
                        elements[j] = elements[j + 1];
                    }
                    nelem--;
                    counter--;
                }
            }
        }
    }
    zero = value;
}

void compress_matrix() {
    refresh_info();
    lines lines_info[maxi + 1 - mini], swap[1];
    values temporary[MAX_ELEMENTS];
    double list_values[MAX_OFFSET];
    unsigned long i, j, k, lines_count, lines[maxi + 1 - mini], control, counter,
    list_index[MAX_OFFSET], list_offset[maxi + 1 - mini], offset, max_offset;
    if (dens > 0.5) {
        printf("dense matrix\n");
    } else {
        memset(list_values, 0, sizeof(list_values));
        memset(list_index, 0, sizeof(list_index));
        memset(list_offset, 0, sizeof(list_offset));
        lines_count = 0; i = 0;
        for (j = mini; j < maxi + 1; j++) {
            control = 0;
            for (k = 0; k < nelem; k++) {
                if (elements[k].line == j) {
                    lines[i] = j;
                    control = 1;
                }
            }
            if (control > 0) {
                lines_count++;
                i++;
            }
        }
    /*    for (i = 0; i < lines_count; i++) {
            printf("%lu ", lines[i]);
        } printf("\n"); */
        for (i = 0; i < lines_count; i++) {
            lines_info[i].index = lines[i];
            lines_info[i].elements = 0;
            for (j = 0; j < nelem; j++) {
                if (elements[j].line == lines[i]) {
                    lines_info[i].elements++;
                }
            }
        }
    /*    for (i = 0; i < lines_count; i++) {
            printf("%lu %lu\n", lines_info[i].index, lines_info[i].elements);
        }   */
        for (i = 0; i < lines_count - 1; i++) {
            for (j = 0; j < lines_count - 1 - i; j++) {
                if (lines_info[j].elements < lines_info[j + 1].elements) {
                    swap[0] = lines_info[j];
                    lines_info[j] = lines_info[j + 1];
                    lines_info[j + 1] = swap[0];
                } else if (lines_info[j].elements == lines_info[j + 1].elements &&
                    lines_info[j].index > lines_info[j + 1].index) {
                    swap[0] = lines_info[j];
                    lines_info[j] = lines_info[j + 1];
                    lines_info[j + 1] = swap[0];
                }
            }
        }
        max_offset = 0;
        for (i = 0; i < lines_count; i++) {
            if (lines_info[i].elements > 0) {
                j = 0;
                while (j < lines_info[i].elements) {
                    for (k = 0; k < nelem; k++) {
                        if (elements[k].line == lines_info[i].index) {
                            temporary[j] = elements[k];
                            j++;
                        }
                    }
                }
                sort(temporary, 1, lines_info[i].elements);
            /*    printf("%lu ", lines_info[i].index);
                for (j = 0; j < lines_info[i].elements; j++) {
                    printf("%f ", temporary[j].value);
                }
                printf("\n");  */
                counter = 0; offset = 0;
                while (counter < lines_info[i].elements) {
                    for (j = 0; j < lines_info[i].elements; j++) {
                        if (list_values[temporary[j].column + offset] == 0) {
                            counter++;
                        } else if ((list_values[temporary[j].column + offset] != 0)) {
                            offset++;
                        }
                    }
                }
                for (j = 0; j < lines_info[i].elements; j++) {
                    list_values[temporary[j].column + offset] = temporary[j].value;
                    list_index[temporary[j].column + offset] = lines_info[i].index;
                }
                list_offset[lines_info[i].index - mini] = offset;
                if (max_offset < offset) {
                    max_offset = offset;
                }
            }
        }
        printf("value =");
        for (i = minj; i < maxj + max_offset + 1; i++) {
            if (list_values[i] == 0) {
                printf(" %.3f", zero);
            } else {
                printf(" %.3f", list_values[i]);
            }
        } printf("\n");
        printf("index =");
        for (i = minj; i < maxj + max_offset + 1; i++) {
            if (list_index[i] == 0) {
                printf(" 0");
            } else {
                printf(" %lu", list_index[i]);
            }
        } printf("\n");
        printf("offset =");
        for (i = 0; i < maxi + 1 - mini; i++) {
            printf(" %lu", list_offset[i]);
        } printf("\n");
    }
}

void save_matrix() {
    int i;
    char default_filename[MAX_FILENAME] = "matrix.txt";
    memmove(filename, filename + 1, strlen(filename));
    filename[strlen(filename) - 1] = 0;
    switch (filename_control) {
        case 0:
            if (strlen(filename) == 0) {
                strcpy(temporary_filename, default_filename);
                break;
            } else {
                strcpy(temporary_filename, filename);
                filename_control = 1;
                break;
            }
        case 1:
            if (strlen(filename) != 0) {
                strcpy(temporary_filename, filename);
                break;
            }
    }
    FILE *write = fopen(temporary_filename, "w");
    for (i = 0; i < nelem; i++) {
        if (elements[i].value != zero) {
            fprintf(write, "%lu %lu %f\n", elements[i].line,
            elements[i].column, elements[i].value);
        }
    }
    fclose(write);
}

/* ------------------------ DEFINIÇÃO DAS FUNÇÕES AUXILIARES ------------------------ */

void calc_mini() {
    unsigned long int i;
    mini = ULONG_MAX;
    for (i = 0; i < nelem; i++) {
        if (mini > elements[i].line && elements[i].value != zero) {
            mini = elements[i].line;
        }
    }
}

void calc_minj() {
    unsigned long int j;
    minj = ULONG_MAX;
    for (j = 0; j < nelem; j++) {
        if (minj > elements[j].column && elements[j].value != zero) {
            minj = elements[j].column;
        }
    }
}

void calc_maxi() {
    unsigned long int i;
    maxi = 0;
    for (i = 0; i < nelem; i++) {
        if (maxi < elements[i].line && elements[i].value != zero) {
            maxi = elements[i].line;
        }
    }
}

void calc_maxj() {
    unsigned long int j;
    maxj = 0;
    for (j = 0; j < nelem; j++) {
        if (maxj < elements[j].column && elements[j].value != zero) {
            maxj = elements[j].column;
        }
    }
}

void calc_size() {
    calc_maxi(); calc_mini(); calc_maxj(); calc_minj();
    size = ((maxi - mini + 1) * (maxj - minj + 1));
}

void calc_dens() {
    calc_size();
    dens = (double)nelem / (double)size;
}

void refresh_info() {
    calc_mini();
    calc_minj();
    calc_maxi();
    calc_maxj();
    calc_size();
    calc_dens();
}

void sort(values array[MAX_ELEMENTS], int option, unsigned long int size) {
    unsigned long int i, j;
    values swap[1];
    refresh_info();
    for (i = 0; i < size - 1; i++) {
        for (j = 0; j < size - i - 1; j++) {
            switch (option) {
                case 0:
                    if (array[j].line > array[j + 1].line) {
                            swap[0] = array[j];
                            array[j] = array[j + 1];
                            array[j + 1] = swap[0];
                    } break;
                case 1:
                    if (array[j].column > array[j + 1].column) {
                            swap[0] = array[j];
                            array[j] = array[j + 1];
                            array[j + 1] = swap[0];
                    } break;
            }
        }
    }
}

/* ----------------------------------------------------------------------------------- */

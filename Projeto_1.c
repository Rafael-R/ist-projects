/* ------------------------------------------------------------------------ */
/*                  Nome: Rafael Rodrigues - Número: 89532                  */
/*                                                                          */
/*                                1º Projeto                                */
/*             Introdução aos Algoritmos e Estruturas de Dados              */
/* ------------------------------------------------------------------------ */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* --------------------------- VARIÁVEIS GLOBAIS --------------------------- */

unsigned long int line, column, mini, minj, maxi, maxj, nelem, size, name_switch, offset[10000];
double value, zero, dens;
char filename[80], temporary_name[80];

struct values {
    unsigned long int line;
    unsigned long int column;
    double value;
};

struct offset {
    double value;
    unsigned long index;
};

struct values list_values[10000];

struct offset list_offset[20000];

/* ------------------- DECLARAÇÃO DAS FUNÇÕES PRINCIPAIS ------------------- */

void add_element(unsigned long int line,unsigned long  int column, double value);

void list_elements();

void info_matrix();

void list_line(unsigned long int line);

void list_column(unsigned long int column);

void sort_matrix(char *parameter_1);

void define_zero(double value);

void compress_matrix();

void save_matrix(char *parameter_2);

/* ------------------- DECLARAÇÃO DAS FUNÇÕES AUXILIARES ------------------- */

void calc_mini();

void calc_minj();

void calc_maxi();

void calc_maxj();

void calc_nelem();

void calc_size();

void calc_dens();

void refresh_info();

void compress_algorithm();

/* --------------------------------- MAIN --------------------------------- */

int main(int argc, char const *argv[]) {
    char command, parameter_1[8], parameter_2[80];
    name_switch = 0; zero = 0;
    FILE *file;
    file = fopen(argv[1], "r");
    if (argc == 2 && file) {
        while (3 == fscanf(file, "%ld %ld %lf", &line, &column, &value)) {
            add_element(line, column, value);
        }
    }
    while (command != 'q') {
        scanf(" %c", &command);
        if (command == 'a') {
            scanf("%ld %ld %lf", &line, &column, &value);
            add_element(line, column, value);
        } else if (command == 'p') {
            list_elements();
        } else if (command == 'i') {
            info_matrix();
        } else if (command == 'l') {
            scanf("%ld", &line);
            list_line(line);
        } else if (command == 'c') {
            scanf("%ld", &column);
            list_column(column);
        } else if (command == 'o') {
            fgets(parameter_1, sizeof parameter_1, stdin);
            sort_matrix(parameter_1);
        } else if (command == 'z') {
            scanf("%lf", &value);
            define_zero(value);
        } else if (command == 's') {
            compress_matrix();
        } else if (command == 'w')   {
            fgets(parameter_2, sizeof parameter_2, stdin);
            save_matrix(parameter_2);
        }
    }
    return 0;
}

/* ------------------- DEFINIÇÃO DAS FUNÇÕES PRINCIPAIS ------------------- */

void add_element(unsigned long int line, unsigned long int column, double value) {
    refresh_info();
    int control = 0;
    if (value == zero) {
        for (size_t i = 0; i < nelem; i++) {
            if (list_values[i].line == line && list_values[i].column == column) {
                for (size_t j = i; j < nelem - 1; j++) {
                    list_values[j].line = list_values[j + 1].line;
                    list_values[j].column = list_values[j + 1].column;
                    list_values[j].value = list_values[j + 1].value;
                }
                list_values[nelem - 1].value = zero;
            }
        }
    } else {
        if (nelem == 0) {
            list_values[nelem].line = line;
            list_values[nelem].column = column;
            list_values[nelem].value = value;
        } else {
            for (size_t i = 0; i < nelem; i++) {
                if (list_values[i].line == line && list_values[i].column == column) {
                    list_values[i].value = value;
                    control = 1;
                }
            }
            if (control == 0) {
                list_values[nelem].line = line;
                list_values[nelem].column = column;
                list_values[nelem].value = value;
            }
        }
    }
}

void list_elements() {
    refresh_info();
    if (list_values[0].value == zero) {
        printf("empty matrix\n");
    } else {
        for (size_t i = 0; i < nelem; i++) {
            printf("[%ld;%ld]=%.3lf\n", list_values[i].line, list_values[i].column, list_values[i].value);
        }
    }
}

void info_matrix() {
    refresh_info();
    printf("[%ld %ld] [%ld %ld] %ld / %ld = %.3f%%\n",
    mini, minj, maxi, maxj, nelem, size, dens * 100);
}

void list_line(unsigned long int line) {
    int control;
    refresh_info();
    if (minj == 0 && maxj == 0) {
        printf("empty line");
    } else {
        while (minj <= maxj) {
            control = 0;
            for (size_t i = 0; i < nelem; i++) {
                if (list_values[i].line == line && list_values[i].column == minj && list_values[i].value != zero) {
                    control = 1;
                    printf(" %.3lf", list_values[i].value);
                }
            }
            if (control == 0) {
                printf(" %.3lf", zero);
            }
            minj++;
        }
    }
    printf("\n");
}

void list_column(unsigned long int column){
    int control;
    refresh_info();
    if (mini == 0 && maxi == 0) {
        printf("empty column");
    } else {
        while (mini <= maxi) {
            control = 0;
            for (size_t i = 0; i < nelem; i++) {
                if (list_values[i].line == mini && list_values[i].column == column && list_values[i].value != zero) {
                    control = 1;
                    printf(" %.3lf", list_values[i].value);
                }
            }
            if (control == 0) {
                    printf(" %.3lf", zero);
            }
            mini++;
        }
    }
    printf("\n");
}

void sort_matrix(char *parameter_1) {
    refresh_info();
    struct values temporary[1];
    char column[7] = "column";
    char* parameter_1_fix = parameter_1 + 1;
    if (parameter_1[0] == '\n') {
        for (size_t i = 0; i < nelem; i++) {
            for (size_t j = i + 1; j < nelem; j++) {
                if (list_values[i].column > list_values[j].column) {
                    temporary[0].line = list_values[i].line;
                    temporary[0].column = list_values[i].column;
                    temporary[0].value = list_values[i].value;
                    list_values[i].line = list_values[j].line;
                    list_values[i].column = list_values[j].column;
                    list_values[i].value = list_values[j].value;
                    list_values[j].line = temporary[0].line;
                    list_values[j].column = temporary[0].column;
                    list_values[j].value = temporary[0].value;
                }
            }
        }
        for (size_t i = 0; i < nelem; i++) {
            for (size_t j = i + 1; j < nelem; j++) {
                if (list_values[i].line > list_values[j].line) {
                    temporary[0].line = list_values[i].line;
                    temporary[0].column = list_values[i].column;
                    temporary[0].value = list_values[i].value;
                    list_values[i].line = list_values[j].line;
                    list_values[i].column = list_values[j].column;
                    list_values[i].value = list_values[j].value;
                    list_values[j].line = temporary[0].line;
                    list_values[j].column = temporary[0].column;
                    list_values[j].value = temporary[0].value;
                }
            }
        }
    } if (strcmp(parameter_1_fix, column) == 0) {
        for (size_t i = 0; i < nelem; i++) {
            for (size_t j = i + 1; j < nelem; j++) {
                if (list_values[i].line > list_values[j].line) {
                    temporary[0].line = list_values[i].line;
                    temporary[0].column = list_values[i].column;
                    temporary[0].value = list_values[i].value;
                    list_values[i].line = list_values[j].line;
                    list_values[i].column = list_values[j].column;
                    list_values[i].value = list_values[j].value;
                    list_values[j].line = temporary[0].line;
                    list_values[j].column = temporary[0].column;
                    list_values[j].value = temporary[0].value;
                }
            }
        }
        for (size_t i = 0; i < nelem; i++) {
            for (size_t j = i + 1; j < nelem; j++) {
                if (list_values[i].column > list_values[j].column) {
                    temporary[0].line = list_values[i].line;
                    temporary[0].column = list_values[i].column;
                    temporary[0].value = list_values[i].value;
                    list_values[i].line = list_values[j].line;
                    list_values[i].column = list_values[j].column;
                    list_values[i].value = list_values[j].value;
                    list_values[j].line = temporary[0].line;
                    list_values[j].column = temporary[0].column;
                    list_values[j].value = temporary[0].value;
                }
            }
        }
    }
}

void define_zero(double value) {
    refresh_info();
    for (size_t i = 0; i < nelem; i++) {
        if (list_values[i].value == value) {
            for (size_t j = i; j < nelem - 1; j++) {
                list_values[j].line = list_values[j + 1].line;
                list_values[j].column = list_values[j + 1].column;
                list_values[j].value = list_values[j + 1].value;
            }
            list_values[nelem - 1].value = value;
            refresh_info();
        }
    }
    zero = value;
}

void compress_matrix() {
    refresh_info();
    if (dens > 0.5) {
        printf("dense matrix\n");
    } else {
        compress_algorithm();
    /*    printf("value =");
        for (size_t i = 0; i < nelem; i++) {
            printf(" %.3lf", list_offset[i].value);
        }
        printf("\n");
        printf("index =");
        for (size_t i = 0; i < nelem; i++) {
            printf(" %ld", list_offset[i].index);
        }
        printf("\n");
        printf("offset =");
        for (size_t i = 0; i < nelem; i++) {
            printf(" %ld", offset[i]);
        }
        printf("\n"); */
    }
}

void save_matrix(char *parameter_2) {
    int i = 0;
    char default_name[80] = "matrix.txt";
    char *name = parameter_2 + 1; name[strlen(name) - 1] = 0;
    if (name_switch == 0) {
        if (parameter_2[0] == '\n' && name_switch == 0) {
            strcpy(filename, default_name);
        } else {
            strcpy(filename, name);
            strcpy(temporary_name, name);
            name_switch++;
        }
    } if (name_switch == 1 && parameter_2[0] == '\n') {
        strcpy(filename, temporary_name);
    }
    FILE *f = fopen(filename, "wb");
    while (list_values[i].value != zero) {
        fprintf(f, "%ld %ld %.3lf\n", list_values[i].line,
        list_values[i].column, list_values[i].value);
        i++;
    }
    fclose(f);
}

/* ------------------- DEFINIÇÃO DAS FUNÇÕES AUXILIARES ------------------- */

void calc_mini() {
    int i = 1;
    if (list_values[0].value == zero) {
        mini = 0;
    } else {
        if (list_values[i].value == zero) {
            mini = list_values[0].line;
        } else {
            mini = list_values[0].line;
            while (list_values[i].value != zero) {
                if (mini > list_values[i].line) {
                    mini = list_values[i].line;
                }
                i++;
            }
        }
    }
}

void calc_minj() {
    int i = 1;
    if (list_values[0].value == zero) {
        minj = 0;
    } else {
        if (list_values[i].value == zero) {
            minj = list_values[0].column;
        } else {
            minj = list_values[0].column;
            while (list_values[i].value != zero) {
                if (minj > list_values[i].column) {
                    minj = list_values[i].column;
                }
                i++;
            }
        }
    }
}

void calc_maxi() {
    int i = 1;
    if (list_values[0].value == zero) {
        maxi = 0;
    } else {
        if (list_values[i].value == zero) {
            maxi = list_values[0].line;
        } else {
            maxi = list_values[0].line;
            while (list_values[i].value != zero) {
                if (maxi < list_values[i].line) {
                    maxi = list_values[i].line;
                }
                i++;
            }
        }
    }
}

void calc_maxj() {
    int i = 1;
    if (list_values[0].value == zero) {
        maxj = 0;
    } else {
        if (list_values[i].value == zero) {
            maxj = list_values[0].column;
        } else {
            maxj = list_values[0].column;
            while (list_values[i].value != zero) {
                if (maxj < list_values[i].column) {
                    maxj = list_values[i].column;
                }
                i++;
            }
        }
    }
}

void calc_nelem() {
    int i = 1;
    if (list_values[0].value == zero) {
        nelem = 0;
    } else {
        nelem = 1;
        while (list_values[i].value != zero) {
            nelem++;
            i++;
        }
    }
}

void calc_size() {
    calc_maxi();
    calc_mini();
    calc_maxj();
    calc_minj();
    if (list_values[0].value == zero) {
        size = 0;
    } else {
        if (list_values[1].value == zero) {
            size = 1;
        } else {
            size = (maxi - mini + 1) * (maxj - minj + 1);
        }
    }
}

void calc_dens() {
    calc_nelem();
    calc_size();
    if (nelem == 0 || size == 0) {
        dens = 0;
    } else {
        dens = (double)nelem / (double)size;
    }
}

void refresh_info() {
    calc_mini();
    calc_minj();
    calc_maxi();
    calc_maxj();
    calc_nelem();
    calc_size();
    calc_dens();
}

void compress_algorithm() {
    refresh_info();
    int line_elements[maxi], max_line_elements, max_line, counter_1;
    for (size_t i = 0; i < maxi; i++) {
        line_elements[i] = 0;
        for (size_t j = 0; j < nelem; j++) {
            if (list_values[j].line == i) {
                line_elements[i]++;
            }
        }
    }
    while (counter_1 < maxi) {
        max_line_elements = line_elements[0];
        counter_1 = 1;
        for (size_t k = 1; k < maxi; k++) {
            if (max_line_elements < line_elements[k]) {
                max_line_elements = line_elements[k];
                max_line = k;
            } if (k == maxi - 1 && line_elements[0] > line_elements[k]) {
                max_line = 0;
            } if (line_elements[k] == 0) {
                counter_1++;
            }
        }
        line_elements[max_line] = 0;
        printf("Maior linha é %d\n", max_line);
        printf("Linhas com zero elementos: %d\n", counter_1);
    }
}

/* ---------------------------------- FIM ---------------------------------- */

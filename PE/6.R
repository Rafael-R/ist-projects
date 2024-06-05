seed <- 1973
n <- 1000
a <- 4
num_components <- 40
threshold <- 126

set.seed(seed)

# Simulação (Abordagem 1)
samples_matrix <- matrix(rexp(n * num_components, rate = 1/a), 
                         nrow = n, ncol = num_components)

# Calcular a soma de cada linha (cada conjunto de 40 componentes)
total_durations <- rowSums(samples_matrix)

# Calcular a proporção de somas que são maiores que o threshold
simulated_probability <- mean(total_durations > threshold)

# Calculo Exato (Abordagem 2)
exact_probability <- 1 - pgamma(threshold, shape = num_components, scale = a)

absolute_difference <- abs(simulated_probability - exact_probability) * 100
absolute_difference <- round(absolute_difference, 4)

# Resultados
absolute_difference

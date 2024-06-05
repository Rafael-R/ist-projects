seed <- 1973
n <- 1000
a <- 4
num_components <- 40
threshold <- 126

set.seed(1973)

# Simulação (Abordagem 1)
simulated_proportions <- replicate(n, {
  samples <- rexp(num_components, rate = 1/a)
  total_duration <- sum(samples)
  total_duration > threshold
})
simulated_probability <- mean(simulated_proportions)

# Calculo Exato (Abordagem 2)
exact_probability <- 1 - pgamma(threshold, shape = num_components, scale = a)

absolute_difference <- abs(simulated_probability - exact_probability) * 100
absolute_difference <- round(absolute_difference, 4)

# Resultados
cat("Probabilidade Simulada: ", round(simulated_probability, 4), "\n")
cat("Probabilidade Exata: ", round(exact_probability, 4), "\n")
cat("Diferença Absoluta Multiplicada por 100: ", absolute_difference, "\n")

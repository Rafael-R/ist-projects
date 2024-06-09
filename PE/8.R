library(pracma)

set.seed(1592)
amostra <- c(31.8, 31.7, 35.2, 37.1, 31.7, 36.1, 36.3, 33.2, 34.3, 37.5, 30.4, 34.6, 32.4, 31.7, 30.2, 34.3, 35.6, 34.9, 38.9)
n <- 12
sub_amostra <- sample(amostra, n, replace = FALSE)

# Calcular os quantis fornecidos e primeiro IC
gamma <- 0.96
a <- qchisq((1 - gamma) / 2, df = n - 1)
b <- qchisq((1 + gamma) / 2, df = n - 1)

variancia_amostral <- var(sub_amostra)
IC_sigma2 <- ( (n - 1) * variancia_amostral ) / c(b, a)

# Otimizar os quantis

optimize_quantis <- function(x) {
  c1 <- x[1]
  d1 <- x[2]
  f1 <- pchisq(d1, df = n - 1) - pchisq(c1, df = n - 1) - gamma
  f2 <- dchisq(d1, df = n + 3) - dchisq(c1, df = n + 3)
  return(c(f1, f2))
}

initial_values <- c(a, b) # valores que vão servir como probabilidades (no vetor x)
optimized_quantis <- fsolve(optimize_quantis, initial_values)
c <- optimized_quantis$x[1]
d <- optimized_quantis$x[2]

IC_sigma2_optimized <- ( (n - 1) * variancia_amostral ) / c(d,c)

# Comparar as amplitudes dos intervalos
amplitude_IC <- diff(IC_sigma2)
amplitude_IC_optimized <- diff(IC_sigma2_optimized)

diferenca_amplitude <- amplitude_IC - amplitude_IC_optimized
diferenca_amplitude
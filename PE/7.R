library(stats4)

# Dados da amostra
amostra <- c(8.54, 4.76, 5.15, 4.96, 6.25, 7.22, 12.9, 6.04, 8.86, 4.88, 6.54,
             4.53, 4.7, 5.38, 5.96, 5.17, 5.09, 5.11)
a <- 4.5

# Função de log-verossimilhança
log_verossimilhanca <- function(theta) {
  if (theta <= 0) return(Inf)
  n <- length(amostra)
  -n * log(theta) + theta * sum(log(amostra - a))
}

# Estimar theta
mle_est <- mle(log_verossimilhanca, start = list(theta = 3.4))
summary(mle_est)

# Obter estimativa de theta
theta_hat <- coef(mle_est)

# Calcular quartil para p = 0.25
p <- 0.25
quartil_25 <- a * (1 - p)^(-1 / theta_hat)

# Calcular valor verdadeiro do quartil para theta = 3.4
theta_true <- 3.4
quartil_true <- a * (1 - p)^(-1 / theta_true)

# Calcular desvio absoluto
desvio_absoluto <- abs(quartil_25 - quartil_true)

# Exibir resultado com 4 casas decimais
round(desvio_absoluto, 4)
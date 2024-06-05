library(pracma)

seed <- 1592
n <- 12
amostra <- c(31.8, 31.7, 35.2, 37.1, 31.7, 36.1, 36.3, 33.2, 34.3, 37.5, 30.4, 
             34.6, 32.4, 31.7, 30.2, 34.3, 35.6, 34.9, 38.9)
gamma <- 0.96


set.seed(seed)

# Selecionar aleatoriamente 12 valores sem reposição
dados <- sample(amostra, 12, replace = FALSE)


a <- qchisq((1 - gamma) / 2, df = n - 1)
b <- qchisq((1 + gamma) / 2, df = n - 1)

objetivo <- function(cd) {
  c <- cd[1]
  d <- cd[2]
  
  eq1 <- pchisq(d, df = n - 1) - pchisq(c, df = n - 1) - gamma
  eq2 <- dchisq(d, df = n + 3) - dchisq(c, df = n + 3)
  return(c(eq1, eq2))
}

# Encontrar os valores de c e d usando fsolve
sol <- fsolve(objetivo, c(a, b))

c <- sol$x[1]
d <- sol$x[2]

# Cálculo das amplitudes dos intervalos de confiança
amplitude_ab <- b - a
amplitude_cd <- d - c

# Diferença entre as amplitudes
abs(round(amplitude_ab - amplitude_cd, 3))


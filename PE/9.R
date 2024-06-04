seed <- 4588
m <- 5000
n <- 100
lambda_H0 <- 2.4
lambda_H1 <- 2.65
k <- 2.623

set.seed(seed)

amostras_H0 <- matrix(rpois(n * m, lambda_H0), nrow = m, ncol = n)
amostras_H1 <- matrix(rpois(n * m, lambda_H1), nrow = m, ncol = n)

erro_tipo_1 <- mean(rowMeans(amostras_H0) > k)  # erro de 1ª espécie
erro_tipo_2 <- mean(rowMeans(amostras_H1) <= k)  # erro de 2ª espécie

quociente <- round(erro_tipo_2 / erro_tipo_1, digits = 2)

cat(quociente, "\n")
seed <- 4588
m <- 5000
n <- 100
lambda_H0 <- 2.4
lambda_H1 <- 2.65
k <- 2.623

set.seed(seed)

erro_1_especie <- 0
erro_2_especie <- 0

for (i in 1:m) {
  amostra_H0 <- rpois(n, lambda_H0)
  media_H0 <- mean(amostra_H0)
  
  if (media_H0 > k) {
    erro_1_especie <- erro_1_especie + 1
  }
  
  amostra_H1 <- rpois(n, lambda_H1)
  media_H1 <- mean(amostra_H1)
  
  if (media_H1 <= k) {
    erro_2_especie <- erro_2_especie + 1
  }
}

prob_erro_1_especie <- erro_1_especie / m
prob_erro_2_especie <- erro_2_especie / m


round(prob_erro_2_especie / prob_erro_1_especie, 2)
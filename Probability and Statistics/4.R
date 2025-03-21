seed <- 2336
n <- 7
i <- 1:9
p <- 45
s <- 650

set.seed(seed)

contador_avisos_nao_desligado <- 0
contador_nao_desligado <- 0

for (x in 1:s) {
  estado <- sample(i, n, replace = TRUE, prob = i / p)
  aviso <- any(estado == 2)
  desligado <- any(estado == 1)

  if (!desligado) {
    contador_nao_desligado <- contador_nao_desligado + 1
  }

  if (aviso && !desligado) {
    contador_avisos_nao_desligado <- contador_avisos_nao_desligado + 1
  }
}

proporcao <- round(
  contador_avisos_nao_desligado / contador_nao_desligado, digits = 2
)

cat(proporcao, "\n")

library(ggplot2)
theme_set(theme_light())

dados <- read.csv("./data/master.csv")
ano <- 2002
grupo_etario <- "55-74 years"

dados <- dados[dados$year == ano & dados$age == grupo_etario, ]

ggplot(dados) +
  geom_boxplot(aes(sex, suicides.100k.pop)) +
  labs(title = paste("Suicide rate in 101 countries in", ano),
       subtitle = paste("In the age group of", grupo_etario),
       y = "Suicides per 100k habitants",
       x = "Sex")

library(ggplot2)

dados <- read.csv("./data/master.csv")
ano <- 2002
grupo_etario <- "55-74 years"

dados <- subset(dados, year == ano & age == grupo_etario)

ggplot(dados) +
  geom_boxplot(aes(x = sex, y = suicides.100k.pop, fill = sex)) +
  labs(title = paste("Suicide rate in 101 countries in", ano),
       subtitle = paste("In the age group of", grupo_etario),
       caption = "Source: kaggle",
       y = "Suicides per 100k habitants",
       x = "Sex") +
  theme_bw() + theme(legend.position = "none")

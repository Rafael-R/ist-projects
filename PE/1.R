library(ggplot2)
theme_set(theme_light())

dados <- read.csv("./data/Paises_PIB_ICH.csv")
continentes <- c("Asia", "Africa")
paises <- c("United Arab Emirates", "Nepal", "Comoros", "Namibia")

dados <- dados[dados$Continent %in% continentes,]

num_paises <- length(unique(dados$Country))
simbolos <- c(0:25,33:(num_paises+6))

ggplot(dados, aes(x = GDP, y = HCI, color = Continent)) +
  geom_point(size = 3, aes(shape = Country)) +
  scale_shape_manual(values = simbolos) +
  scale_x_log10() +
  geom_text(data = subset(dados, Country %in% paises),
            aes(label = Country), vjust = -1, hjust = 0.5, size = 4) +
  labs(title = "HCI vs GPD in Asian and African countries",
       x = "GDP per capita [2023]",
       y = "Human Capital Index [2020]")

library(ggplot2)

dados <- read.csv("./data/Paises_PIB_ICH.csv")
continentes <- c("Asia", "Africa")
paises <- c("Comoros", "Namibia", "Nepal", "United Arab Emirates")

dados <- subset(dados, Continent %in% continentes)
dados$Shape <- ifelse(dados$Country %in% paises, dados$Country, "Other")

ggplot(dados, aes(x = GDP, y = HCI)) +
  geom_point(size = 3, aes(color = Continent, shape = Shape)) +
  scale_shape_manual(values = c(9, 13, 7, 10, 19), breaks = c(paises, "Other")) +
  scale_x_log10() +
  geom_text(data = subset(dados, Country %in% paises), aes(label = Country),
            vjust = -1, hjust = 0.5, size = 3.5) +
  labs(title = "Human Capital Index relative to GDP per capita",
       subtitle = "In Asian and African countries",
       caption = "Source: World Bank/IMF",
       shape = "Country")
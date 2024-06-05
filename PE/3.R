library(ggplot2)

dados <- readxl::read_xlsx("./data/electricity.xlsx")
tipo <- "Renewables"
ano <- 2015
paises <- c("IEA Total", "Hungary", "Iceland")

dados <- subset(dados, YEAR >= ano & COUNTRY %in% paises & PRODUCT == tipo)
dados$share <- as.numeric(dados$share) * 100
dados$tempo <- as.Date(paste(dados$YEAR, dados$MONTH, "01", sep = "-"))

ggplot(dados) +
  geom_line(size = 1.5, aes(x = tempo, y = share, color = COUNTRY)) +
  ylim(0, 100) +
  labs(title = paste("Evolution of the share of", tipo),
       subtitle = paste("Relative to other energy sources, from", ano),
       y = "Share (%)", x = "Year", color = "Country")

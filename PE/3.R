library(ggplot2)

dados <- readxl::read_xlsx("./data/electricity.xlsx")
tipo <- "Renewables"
ano <- 2015
paises <- c("IEA Total", "Hungary", "Iceland")

dados <- subset(dados, YEAR >= ano & COUNTRY %in% paises & PRODUCT == tipo)
dados$share <- as.numeric(dados$share) * 100
dados$date <- as.Date(paste(dados$YEAR, dados$MONTH, "01", sep = "-"))

ggplot(dados, aes(x = date, y = share, color = COUNTRY)) +
  geom_line(linewidth = 1) +
  geom_point() +
  ylim(0, 100) +
  labs(title = paste("Evolution of the share of", tipo),
       subtitle = paste("Relative to other energy sources, from", ano),
       y = "Share (%)", x = "Month", color = "Country")

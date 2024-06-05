library(pracma)

seed <- 1592
amostra <- c(31.8, 31.7, 35.2, 37.1, 31.7, 36.1, 36.3, 33.2, 34.3, 37.5, 30.4, 
             34.6, 32.4, 31.7, 30.2, 34.3, 35.6, 34.9, 38.9)

set.seed(seed)

n <- 12
gamma <- 0.96
df <- n - 1
s2 <- var(amostra)

alpha <- (1 - gamma) / 2
chi2_lower <- qchisq(alpha, df)
chi2_upper <- qchisq(1 - alpha, df)

lower_bound <- (df * s2) / chi2_upper
upper_bound <- (df * s2) / chi2_lower

a <- qchisq((1 - gamma) / 2, df)
b <- qchisq((1 + gamma) / 2, df)

F <- function(c, d, n, gamma) {
  df_c <- df * c
  df_d <- df * d
  eq1 <- pchisq(df_d, df + 2) - pchisq(df_c, df + 2) - gamma
  eq2 <- dchisq(df_d, df + 2) - dchisq(df_c, df + 2)
  return(c(eq1, eq2))
}

sol <- fsolve(F, c(a, b), n = n, gamma = gamma)

c <- sol$x[1]
d <- sol$x[2]

lower_bound_new <- (df * s2) / d
upper_bound_new <- (df * s2) / c

amplitude_original <- upper_bound - lower_bound
amplitude_novo <- upper_bound_new - lower_bound_new

round(amplitude_original - amplitude_novo, 3)


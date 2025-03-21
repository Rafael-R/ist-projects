seed <- 2126
n <- 4
r <- 150
m <- 130
threshold <- -0.9

set.seed(2126)

generate_T <- function(n) {
  Z <- rnorm(n + 1)
  T <- sqrt(n) * Z[1] / sqrt(sum(Z[-1]^2))
  return(T)
}

proportions <- numeric(r)
for (i in 1:r) {
  sample_T <- replicate(m, generate_T(n))
  proportions[i] <- mean(sample_T <= threshold)
}

empirical_p <- mean(proportions)

theoretical_p <- pt(threshold, df = n)

difference <- abs(empirical_p - theoretical_p) * 100

round(difference, 5)

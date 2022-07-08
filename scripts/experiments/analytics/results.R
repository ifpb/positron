# Packages needed
packages_needed <- c("ggplot2", "Rmisc", "tidyverse", "colorspace", 
                     "rcartocolor", "ggforce", "ggdist", "ggridges",
                     "ggbeeswarm", "gghalves", "systemfonts")

# Install packages not yet installed
packages_installed <- packages_needed %in% rownames(installed.packages())
if (any(packages_installed == FALSE)) {
  print(paste(packages_needed[!packages_installed], 
              "not installed!!", sep = " "))
  quit()
} 

packages_loaded <- packages_needed %in% .packages()
if (any(packages_loaded == FALSE)) {
  invisible(lapply(packages_needed[!packages_loaded], 
                   library, character.only = TRUE))
  rm(packages_installed, packages_loaded, packages_needed)
}

# Need a symbolic link to wherever folder it is located: $ ln -s /path/to/positron ~/positron
working_directory <- "~/positron/scripts/experiments/analytics"
if (getwd() != working_directory) setwd("~/positron/scripts/experiments/analytics")
rm(working_directory)

rm(list = ls())

data <- data.frame( matrix(ncol = 3, nrow = 150) )
colnames(data) <- c("nodes", "policies", "values")

level_order <- c("60", "90", "120", "150", "180")

data$nodes <- c(rep(c("60"), 30), rep(c("90"), 30), rep(c("120"), 30), rep(c("150"), 30), rep(c("180"), 30))
data$policies <- c(rep("sat", 75), rep("bal", 75))
data$values <- c(rnorm(75, mean = 50, sd = 10), rnorm(75, mean = 70, sd = 10))

pdf(file="plot.pdf")
plot <- ggplot( data, 
                aes(
                    x=factor(nodes, level = level_order), 
                    y=values, 
                    fill=policies 
                )
              ) +
    theme_bw() +
    geom_boxplot (
        width = 0.2, fill = "white",
        size = 1.5, outlier.shape = NA
    ) +
    ggdist::stat_halfeye(
        adjust = .33, ## bandwidth
        width = .67,
        color = NA, ## remove slab interval
        position = position_nudge(x = .15)
    ) +
    gghalves::geom_half_point(
        side = "l",
        range_scale = .3,
        alpha = .5, size = 3
    ) +
    facet_grid(~policies) +
    theme(
    legend.position = "top",
    legend.text = element_text(face = "bold", size = 12),
    legend.title = element_text(face = "bold", size = 12),
    axis.text = element_text(face = "bold", size = 12),
    axis.title = element_text(face = "bold", size = 12),
    strip.text = element_text(face = "bold", size = 12)
  )
dev.off()
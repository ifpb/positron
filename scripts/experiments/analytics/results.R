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

data <- data.frame( matrix(ncol = 4, nrow = 0) )
colnames(data) <- c("policies", "nodes", "turn", "values")

level_order <- c("30", "60", "90", "120", "150", "180")

for (i in level_order) {
  auxfile <- read.csv(paste("../results/", "pfair-", i, "nodes.txt", sep = ""))
  auxDFSat <- data.frame(
    policies=rep("sat", length(auxfile$pfSat)),
    nodes=rep(i, length(auxfile$pfSat)),
    turn=seq(1, length(auxfile$pfSat)),
    values=auxfile$pfSat
    )
  auxDFBal <- data.frame(
    policies=rep("bal", length(auxfile$pfBal)),
    nodes=rep(i, length(auxfile$pfBal)),
    turn=seq(1, length(auxfile$pfBal)),
    values=auxfile$pfBal
    )
  data <- rbind(data, auxDFBal)
  data <- rbind(data, auxDFSat)
}
rm(i)

# pdf(file="plot.pdf")
plot <- ggplot( data, 
                aes(
                    x=factor(nodes, level = level_order), 
                    y=values, 
                    fill=policies 
                )
              ) +
  theme_bw() +
  theme_bw() +
  geom_bar(stat = "identity") +
  geom_text(position = position_stack(vjust = 0.5)) +
  scale_fill_manual(name = "Tipos de nós:", 
                    labels = c("Grupo 1", "Grupo 2", "Grupo 3"),
                    values = c("gray","#E69F00","#56B4E9")
                    ) +
  labs(title = "", x = "Quantidade de nós", 
       y =  "Percentual de alocação justa (%)") +
  facet_grid(~politica, 
             labeller = labeller(
               politica = c(bal = "Balanceamento", sat = "Saturação")
               )
             ) +
  theme(
    legend.position = "top",
    legend.text = element_text(face = "bold", size = 12),
    legend.title = element_text(face = "bold", size = 12),
    axis.text = element_text(face = "bold", size = 12),
    axis.title = element_text(face = "bold", size = 12),
    strip.text = element_text(face = "bold", size = 12)
  )
  #   theme_bw() +
  #   geom_boxplot (
  #       width = 0.2, fill = "white",
  #       size = 1.5, outlier.shape = NA
  #   ) +
  #   ggdist::stat_halfeye(
  #       adjust = .33, ## bandwidth
  #       width = .67,
  #       color = NA, ## remove slab interval
  #       position = position_nudge(x = .15)
  #   ) +
  #   gghalves::geom_half_point(
  #       side = "l",
  #       range_scale = .3,
  #       alpha = .5, size = 3
  #   ) +
  #   facet_wrap(~policies, scales="free_y") +
  #   theme(
  #   legend.position = "top",
  #   legend.text = element_text(face = "bold", size = 12),
  #   legend.title = element_text(face = "bold", size = 12),
  #   axis.text = element_text(face = "bold", size = 12),
  #   axis.title = element_text(face = "bold", size = 12),
  #   strip.text = element_text(face = "bold", size = 12)
  # )
# dev.off()
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

data <- data.frame( matrix(ncol = 5, nrow = 0) )
colnames(data) <- c("policies", "nodes",  "groups", "turns", "values")

scenarios <- c("30", "60", "90", "120", "150", "180")
policies <- c("bal", "sat")
groups <- seq(1,4)
turns <- seq(1,30)

for (policie in policies) {
  for (scenario in scenarios) {
    for (turn in turns) {

      filename <- paste("../results/", "pfair-", scenario, "nodes-", policie, "-", turn, ".csv", sep = "")
      auxFile <- read.csv(filename)

      for (group in groups) {

        auxDF <- data.frame(
          policies=policie, 
          nodes=scenario,
          groups=group,
          turns=turn, 
          values=ifelse(is.na(auxFile$pfair[group]), 0, auxFile$pfair[group])
          )
        data <- rbind(data, auxDF)
      
      }
    }
  }
}

rm(auxFile, auxDF, filename, group, policie, scenario, turn)

dataAvg <- data.frame( matrix(ncol = 4, nrow = 0) )
colnames(dataAvg) <- c("policies", "nodes", "groups", "means")

for (policie in policies) {
  for (scenario in scenarios) {
    for (group in groups) {
      
      auxDF <- data.frame(
        policies=policie, 
        nodes=scenario,
        groups=group,
        means=mean(data[data$policies == policie & data$nodes == scenario & data$groups == group,]$values)
      )

      dataAvg <- rbind(dataAvg, auxDF)

    }
  }
}

rm(auxDF, policie, scenario, group)

gplotPfair <- ggplot(dataAvg, 
                     aes(x = factor(nodes, level = scenarios),
                         y=means,
                         fill=factor(-groups),
                         label = round(means, digits = 1)
                         )
                    ) +
  theme_bw() +
  geom_bar(stat = "identity") +
  geom_text(position = position_stack(vjust = 0.5)) +
  scale_fill_manual(name = "Allocated groups:", 
                    labels = c("Unfair", "Group 1", "Group 2", "Group 3"),
                    values = c("red", "gray","#E69F00","#56B4E9")
                    ) +
  labs(title = "", x = "Total amount of workers", 
       y =  "Percentage of fair allocation (%)") +
  # coord_flip() +
  facet_grid(~policies, 
             labeller = labeller(
               policies = c(bal = "Balanced", sat = "Saturation")
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
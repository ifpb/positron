rm(list = ls())

powerScenario <- "powerless"

# Packages needed
packages_needed <- c("ggplot2", "Rmisc", "tidyverse", "colorspace", 
                     "rcartocolor", "ggforce", "ggdist", "ggridges",
                     "ggbeeswarm", "gghalves", "systemfonts")

# Install packages not yet installed
packages_installed <- packages_needed %in% rownames(installed.packages())
if (any(packages_installed == FALSE)) {
  print(paste(packages_needed[!packages_installed], " not installed!!", sep = " "))
  quit()
} 

packages_loaded <- packages_needed %in% .packages()
if (any(packages_loaded == FALSE)) {
  invisible(
    lapply(
      packages_needed[!packages_loaded], library, character.only = TRUE
    )
  )
}
rm(packages_installed, packages_loaded, packages_needed)

# Need a symbolic link to wherever folder it is located: $ ln -s /path/to/positron ~/positron
working_directory <- "~/positron/scripts/experiments/analytics"
if (getwd() != working_directory) setwd("~/positron/scripts/experiments/analytics")
rm(working_directory)

data <- data.frame( matrix(ncol = 5, nrow = 0) )
colnames(data) <- c("policies", "loss", "nodes",  "turns", "values")
auxBalDF <- data.frame( matrix(ncol = 5, nrow = 0) )
colnames(auxBalDF) <- c("policies", "loss", "nodes",  "turns", "values")
auxSatDF <- data.frame( matrix(ncol = 5, nrow = 0) )
colnames(auxSatDF) <- c("policies", "loss", "nodes",  "turns", "values")

scenarios <- c("30", "60", "90", "120", "150", "180")
lossScenarios <- c("10", "20", "25", "30", "50")

for (lossScenario in lossScenarios) {
  for (scenario in scenarios) {

    filename <- paste("../results/JISA/powerless/", lossScenario, "/makespan-", scenario, "nodes.txt", sep = "")
    auxFile <- read.csv(filename)

    auxDF <- data.frame(
      policies=rep("bal", length(auxFile$msBal)), 
      loss=rep(lossScenario, length(auxFile$msBal)),
      nodes=rep(scenario, length(auxFile$msBal)),
      turns=seq(1, length(auxFile$msBal)),
      values=auxFile$msBal
    )
    auxBalDF <- rbind(auxBalDF, auxDF)

    auxDF <- data.frame(
      policies=rep("sat", length(auxFile$msSat)),
      loss=rep(lossScenario, length(auxFile$msSat)), 
      nodes=rep(scenario, length(auxFile$msSat)),
      turns=seq(1, length(auxFile$msSat)),
      values=auxFile$msSat
    )
    auxSatDF <- rbind(auxSatDF, auxDF)

  }
}

data <- rbind(data, auxBalDF)
data <- rbind(data, auxSatDF)

data$values <- (1 - data$values) * 100

rm(filename, auxFile, auxDF, scenario, auxBalDF, auxSatDF, lossScenario)

dataAvg <- data.frame( matrix(ncol = 6, nrow = 0) )
colnames(dataAvg) <- c("policies", "loss", "nodes", "upper", "mean", "lower")
auxBalAvg <- data.frame( matrix(ncol = 6, nrow = 0) )
colnames(auxBalAvg) <- c("policies", "loss", "nodes", "upper", "mean", "lower")
auxSatAvg <- data.frame( matrix(ncol = 6, nrow = 0) )
colnames(auxSatAvg) <- c("policies", "loss", "nodes", "upper", "mean", "lower")

for (lossScenario in lossScenarios) {
  for (scenario in scenarios) {
      
      auxVector <- Rmisc::CI(data[data$policies == "bal" & data$loss == lossScenario & data$nodes == scenario,]$values)
      auxDF <- data.frame(
        policies="bal",
        loss=lossScenario, 
        nodes=scenario,
        upper=as.numeric(auxVector[1]),
        mean=as.numeric(auxVector[2]),
        lower=as.numeric(auxVector[3])
      )
      auxBalAvg <- rbind(auxBalAvg, auxDF)

      auxVector <- Rmisc::CI(data[data$policies == "sat" & data$loss == lossScenario & data$nodes == scenario,]$values)
      auxDF <- data.frame(
        policies="sat",
        loss=lossScenario, 
        nodes=scenario,
        upper=as.numeric(auxVector[1]),
        mean=as.numeric(auxVector[2]),
        lower=as.numeric(auxVector[3])
      )
      auxSatAvg <- rbind(auxSatAvg, auxDF)

  }
}

dataAvg <- rbind(dataAvg, auxBalAvg)
dataAvg <- rbind(dataAvg, auxSatAvg)

rm(auxDF, auxBalAvg, auxSatAvg, auxVector, scenario, lossScenario)

for (lossScenario in lossScenarios) {

  gplotMakespan <- ggplot(dataAvg[dataAvg$loss == lossScenario,], 
                       aes(x = factor(nodes, level = scenarios), 
                       y=mean, 
                       fill=policies
                       )
                      ) +
  theme_bw() +
  geom_bar(stat = "identity", position = position_dodge(), alpha=0.6) +
  scale_fill_manual("Policies:", labels = c("Balancing", "Saturation"), 
                    values = c("#FB9A99", "#CAB2D6")) +
  scale_y_continuous(limits = c(0,11.5)) +                    
  geom_errorbar(aes(ymin=lower, ymax=upper), width=.2,
                position = position_dodge(.9)) +
  labs(title = "", x = "Total amount of Workers", 
        y =  "% of makespan increase") +
  theme(
    legend.position = "top",
    legend.text = element_text(face = "bold", size = 20),
    legend.title = element_text(face = "bold", size = 20),
    axis.text = element_text(face = "bold", size = 20),
    axis.title = element_text(face = "bold", size = 20),
    strip.text = element_text(face = "bold", size = 20)
  )
  ggsave(file=paste("makespan-powerless-", lossScenario, ".pdf", sep=""), height=5.0, width=10.0, units="in")

}
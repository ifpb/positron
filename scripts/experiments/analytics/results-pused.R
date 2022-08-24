rm(list = ls())

powerScenario <- readline(prompt="\nEnter power scenario (powerfull|powerless): ")
if ( powerScenario != "powerfull" & powerScenario != "powerless" ) {
  print("Error: invalid power scenario!!")
  quit()
}

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

data <- data.frame( matrix(ncol = 4, nrow = 0) )
colnames(data) <- c("policies", "nodes",  "turns", "values")
auxBalDF <- data.frame( matrix(ncol = 4, nrow = 0) )
colnames(auxBalDF) <- c("policies", "nodes",  "turns", "values")
auxSatDF <- data.frame( matrix(ncol = 4, nrow = 0) )
colnames(auxSatDF) <- c("policies", "nodes",  "turns", "values")

scenarios <- c("30", "60", "90", "120", "150", "180")

for (scenario in scenarios) {

  filename <- paste("../results/", powerScenario, "/pused-", scenario, "nodes.txt", sep = "")
  auxFile <- read.csv(filename)

  auxDF <- data.frame(
    policies=rep("bal", length(auxFile$puBal)), 
    nodes=rep(scenario, length(auxFile$puBal)),
    turns=seq(1, length(auxFile$puBal)),
    values=auxFile$puBal
  )
  auxBalDF <- rbind(auxBalDF, auxDF)

  auxDF <- data.frame(
    policies=rep("sat", length(auxFile$puSat)), 
    nodes=rep(scenario, length(auxFile$puSat)),
    turns=seq(1, length(auxFile$puSat)),
    values=auxFile$puSat
  )
  auxSatDF <- rbind(auxSatDF, auxDF)

}

data <- rbind(data, auxBalDF)
data <- rbind(data, auxSatDF)

rm(filename, auxFile, auxDF, scenario, auxBalDF, auxSatDF)

dataAvg <- data.frame( matrix(ncol = 5, nrow = 0) )
colnames(dataAvg) <- c("policies", "nodes", "upper", "mean", "lower")
auxBalAvg <- data.frame( matrix(ncol = 5, nrow = 0) )
colnames(auxBalAvg) <- c("policies", "nodes", "upper", "mean", "lower")
auxSatAvg <- data.frame( matrix(ncol = 5, nrow = 0) )
colnames(auxSatAvg) <- c("policies", "nodes", "upper", "mean", "lower")

for (scenario in scenarios) {
    
    auxVector <- Rmisc::CI(data[data$policies == "bal" & data$nodes == scenario,]$values)
    auxDF <- data.frame(
      policies="bal", 
      nodes=scenario,
      upper=as.numeric(auxVector[1]),
      mean=as.numeric(auxVector[2]),
      lower=as.numeric(auxVector[3])
    )
    auxBalAvg <- rbind(auxBalAvg, auxDF)

    auxVector <- Rmisc::CI(data[data$policies == "sat" & data$nodes == scenario,]$values)
    auxDF <- data.frame(
      policies="sat", 
      nodes=scenario,
      upper=as.numeric(auxVector[1]),
      mean=as.numeric(auxVector[2]),
      lower=as.numeric(auxVector[3])
    )
    auxSatAvg <- rbind(auxSatAvg, auxDF)

}

dataAvg <- rbind(dataAvg, auxBalAvg)
dataAvg <- rbind(dataAvg, auxSatAvg)

rm(auxDF, auxBalAvg, auxSatAvg, auxVector, scenario)

gplotPused <- ggplot(dataAvg, 
                     aes(x = factor(nodes, level = scenarios), 
                     y=mean, 
                     fill=policies
                     )
                    ) +
theme_bw() +
geom_bar(stat = "identity", position = position_dodge(), alpha=0.6) +
scale_fill_manual("Policies:", labels = c("Balancing", "Saturation"), 
                  values = c("#FB9A99", "#CAB2D6")) +
geom_errorbar(aes(ymin=lower, ymax=upper), width=.2,
              position = position_dodge(.9)) +
labs(title = "", x = "Total amount of workers", 
      y =  "Percentage of used workers (%)") +
theme(
  legend.position = "top",
  legend.text = element_text(face = "bold", size = 20),
  legend.title = element_text(face = "bold", size = 20),
  axis.text = element_text(face = "bold", size = 20),
  axis.title = element_text(face = "bold", size = 20),
  strip.text = element_text(face = "bold", size = 20)
)
ggsave(file=paste("pused-", powerScenario, ".pdf", sep=""), height=7.0, width=10.0, units="in")
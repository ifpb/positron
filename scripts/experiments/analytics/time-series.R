rm(list = ls())

# powerScenario <- readline(prompt="\nEnter power scenario (powerfull|powerless): ")
# if ( powerScenario != "powerfull" & powerScenario != "powerless" ) {
#   print("Error: invalid power scenario!!")
#   quit()
# }

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

g1Data <- read.csv("g1_times.csv", header = F)
colnames(g1Data) <- c("arrival", "departure")
g1Aux <- data.frame(matrix(nrow = 0, ncol = 2))
g1Aux <- rbind( g1Aux, c(0, 0) )
for ( i in 1:length(g1Data$arrival) ) {
  g1Aux <- rbind( g1Aux, c(g1Data$arrival[i], 1) )
  g1Aux <- rbind( g1Aux, c(g1Data$departure[i], -1) )
}
colnames(g1Aux) <- c("time", "flag")
g1Temp <- g1Aux[order(g1Aux$time),]
value <- cumsum(g1Temp$flag)
g1Final <- cbind(g1Temp, value)
gType <- rep("Group 1", nrow(g1Final))
g1Final <- cbind(gType, g1Final)
rm(g1Aux, g1Temp, i, value, gType)

g2Data <- read.csv("g2_times.csv", header = F)
colnames(g2Data) <- c("arrival", "departure")
g2Aux <- data.frame(matrix(nrow = 0, ncol = 2))
g2Aux <- rbind( g2Aux, c(0, 0) )
for ( i in 1:length(g2Data$arrival) ) {
  g2Aux <- rbind( g2Aux, c(g2Data$arrival[i], 1) )
  g2Aux <- rbind( g2Aux, c(g2Data$departure[i], -1) )
}
colnames(g2Aux) <- c("time", "flag")
g2Temp <- g2Aux[order(g2Aux$time),]
value <- cumsum(g2Temp$flag)
g2Final <- cbind(g2Temp, value)
gType <- rep("Group 2", nrow(g2Final))
g2Final <- cbind(gType, g2Final)
rm(g2Aux, g2Temp, i, value, gType)

g3Data <- read.csv("g3_times.csv", header = F)
colnames(g3Data) <- c("arrival", "departure")
g3Aux <- data.frame(matrix(nrow = 0, ncol = 2))
g3Aux <- rbind( g3Aux, c(0, 0) )
for ( i in 1:length(g3Data$arrival) ) {
  g3Aux <- rbind( g3Aux, c(g3Data$arrival[i], 1) )
  g3Aux <- rbind( g3Aux, c(g3Data$departure[i], -1) )
}
colnames(g3Aux) <- c("time", "flag")
g3Temp <- g3Aux[order(g3Aux$time),]
value <- cumsum(g3Temp$flag)
g3Final <- cbind(g3Temp, value)
gType <- rep("Group 3", nrow(g3Final))
g3Final <- cbind(gType, g3Final)
rm(g3Aux, g3Temp, i, value, gType)

gFinal <- rbind(g1Final, g2Final)
gFinal <- rbind(gFinal, g3Final)
rownames(gFinal) <- 1:nrow(gFinal)

# gTimeSeries <- ggplot(gFinal, aes(x=(time), y=gType, fill=gType)) +
#   theme_bw() +
#   geom_density_ridges(alpha=0.7) +
#   scale_fill_manual(name = "Group of Workers:", 
#     labels = c("Group 1", "Group 2", "Group 3"),
#     values = c("gray","#E69F00","#56B4E9")
#   ) +
#   labs(title = "", x = "Time (h)", 
#        y =  "Density of running applications per group of workers") +
#   theme(
#     legend.position = "none",
#     axis.text = element_text(face = "bold", size = 12),
#     axis.title = element_text(face = "bold", size = 12),
#     strip.text = element_text(face = "bold", size = 12)
#   )
#   # ggsave(file=paste("pfair-", powerScenario, ".pdf", sep=""))

gTimeSeries <- ggplot(gFinal, aes(x=(time/3600), y=value, fill=gType)) +
  theme_bw() +
  geom_area(alpha=0.5) +
   scale_fill_manual(name = "Group of Workers:", 
    labels = c("Group 1", "Group 2", "Group 3"),
    values = c("gray","#E69F00","#56B4E9")
  ) +
  labs(title = "", x = "Time (h)", y =  "Running applications per group of workers") +
  # ylim(0,31) +
  theme(
    # legend.position = c(0.2, 0.8),
    legend.position = "top",
    legend.text = element_text(face = "bold", size = 14),
    legend.title = element_text(face = "bold", size = 14),
    axis.text = element_text(face = "bold", size = 14),
    axis.title = element_text(face = "bold", size = 14),
    strip.text = element_text(face = "bold", size = 14)
  )
  ggsave(file="time-series.pdf")
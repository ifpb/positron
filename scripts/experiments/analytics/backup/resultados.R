# Packages needed
packages_needed <- c("ggplot2", "Rmisc")

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
working_directory <- "~/positron/scripts/experiments/analytics/backup"
if (getwd() != working_directory) setwd("~/positron/scripts/experiments/analytics/backup")
rm(working_directory)

rm(list = ls())
auxDF <- read.csv("primeiro.csv", header = T)
colnames(auxDF) <- c("nodes", "6", "9", "12", "15", "18")

rownames(auxDF) <- auxDF$nodes
auxDF$nodes <- NULL
auxDF <- as.matrix(auxDF)
auxDF <- as.data.frame(t(auxDF))
rownames(auxDF) <- NULL
nodes <- c("6", "9", "12", "15", "18")
auxDF <- cbind(nodes, auxDF)
rm(nodes)

balanceado <- auxDF[,1:4]
colnames(balanceado) <- c("nodes", "avg", "min", "max")
politica <- rep("bal", 5)
balanceado <- cbind(politica, balanceado)

saturado <- auxDF[,c(1,5:7)]
colnames(saturado) <- c("nodes", "avg", "min", "max")
politica <- rep("sat", 5)
saturado <- cbind(politica, saturado)

primeiro <- rbind(balanceado, saturado)
rm(auxDF, balanceado, politica, saturado)

balanceado <- read.csv("balanceado.csv")
saturado <- read.csv("saturacao.csv")

colnames(balanceado) <- c("nodes", "6", "9", "12", "15", "18")
colnames(saturado) <- c("nodes", "6", "9", "12", "15", "18")

rownames(balanceado) <- balanceado$nodes
rownames(saturado) <- saturado$nodes
balanceado$nodes <- NULL
saturado$nodes <- NULL
balanceado <- as.matrix(balanceado)
saturado <- as.matrix(saturado)
balanceado <- as.data.frame(t(balanceado))
saturado <- as.data.frame(t(saturado))
rownames(balanceado) <- NULL
rownames(saturado) <- NULL
nodes <- c("6", "9", "12", "15", "18")
balanceado <- cbind(nodes, balanceado)
saturado <- cbind(nodes, saturado)
rm(nodes)

politica <- rep("bal", 5)
grupo <- rep("1",5)
g1bal <- balanceado[,1:4]
colnames(g1bal) <- c("nodes", "avg", "min", "max")
g1bal <- cbind(politica,grupo,g1bal)
grupo <- rep("2",5)
g2bal <- balanceado[,c(1,5:7)]
colnames(g2bal) <- c("nodes", "avg", "min", "max")
g2bal <- cbind(politica,grupo,g2bal)
grupo <- rep("3",5)
g3bal <- balanceado[,c(1,8:10)]
colnames(g3bal) <- c("nodes", "avg", "min", "max")
g3bal <- cbind(politica,grupo,g3bal)

politica <- rep("sat", 5)
grupo <- rep("1",5)
g1sat <- saturado[,1:4]
colnames(g1sat) <- c("nodes", "avg", "min", "max")
g1sat <- cbind(politica,grupo,g1sat)
grupo <- rep("2",5)
g2sat <- saturado[,c(1,5:7)]
colnames(g2sat) <- c("nodes", "avg", "min", "max")
g2sat <- cbind(politica,grupo,g2sat)
grupo <- rep("3",5)
g3sat <- saturado[,c(1,8:10)]
colnames(g3sat) <- c("nodes", "avg", "min", "max")
g3sat <- cbind(politica,grupo,g3sat)

segundo <- rbind(g1bal,g2bal,g3bal,g1sat,g2sat,g3sat)

rm(balanceado, saturado, g1bal, g1sat, g2bal, 
   g2sat, g3bal, g3sat, politica, grupo)

level_order <- c("6", "9", "12", "15", "18")

grafico1 <- ggplot(primeiro, aes(x = factor(nodes, level = level_order), 
                                 y=avg, fill=politica)) +
  theme_bw() +
  geom_bar(stat = "identity", position = position_dodge()) +
  scale_fill_manual("Políticas:", labels = c("Balanceamento", "Saturação"), 
                    values = c("#FB9A99", "#CAB2D6")) +
  geom_errorbar(aes(ymin=min, ymax=max), width=.2,
                position = position_dodge(.9)) +
  labs(title = "", x = "Quantidade de nós", 
       y =  "Percentual de nós utilizados (%)") +
  theme(
    legend.position = "top",
    legend.text = element_text(face = "bold", size = 12),
    legend.title = element_text(face = "bold", size = 12),
    axis.text = element_text(face = "bold", size = 12),
    axis.title = element_text(face = "bold", size = 12)
    )
print(grafico1)

grafico2 <- ggplot(segundo, aes(x = factor(nodes, level = level_order),
                                y=avg/3, fill=grupo, 
                                label = round(avg/3, digits = 1)
                                )
                   ) +
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
print(grafico2)

POSITRON: Fair Allocation Scheme for Multifunctional IoT Devices
================================

## Table of Contents:

1) [Information](#information)
2) [Installation](#installation)
3) [Utilization](#utilization)
4) [Contacts](#contacts)

## Information

The POSITRON was implemented and tested based on version 3.38 of [NS-3](https://www.nsnam.org), in a Ubuntu 22.04 Linux environment.

For its operation, prior installation of SQLite 3 and the YAML CPP library is required.

Installation and usage instructions are as follows.

## Installation

Firstly, you should clone this repository using the Git tool

```shell
git clone https://github.com/ifpb/positron
```

After cloning the repository, it is recommended to install the dependencies

```shell
cd positron 
bash scripts/dependencies.sh
```

The installation should proceed according to the steps provided by NS-3 itself

```shell
./ns3 configure --enable-example --enable-tests
```

followed by

```shell
./ns3 build
```

## Utilization

Once compiled, you can run POSITRON from the main.cc and input.yaml files in the scratch directory.

```shell
./ns3 run main 
```

Simply modify the input.yaml file to configure new simulation scenarios.

Examples of simulation parameters

```shell
./ns3 run "main --seed=79" 
```

```shell
./ns3 run "main --logging=true --tracing=true"
```

```shell
./waf run "main --balanced=true"
```

## Contacts

* Aldri Santos (aldri[at]dcc.ufmg.br)
* Ayrton M. Porto de Gois (ayrton.porto[at]academico.ifpb.edu.br)
* Diego M. Rocha (diego.rocha[at]academico.ifpb.edu.br)
* Fernando Matos (fernando[at]ci.ufpb.br)
* Jefferson Lucas Ferreira da Silva (ferreira.jefferson[at]academico.ifpb.edu.br)
* Leandro H. Batista da silva (leandro.batista[at]academico.ifpb.edu.br)
* Paulo Ditarso Maciel Jr. (paulo.maciel[at]ifpb.edu.br)
* Ricardo Pereira Lins (ricardo.lins[at]academico.ifpb.edu.br)

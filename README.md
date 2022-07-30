
POSITRON: Esquema de Alocação Justa de Dispositivos IoT Multifuncionais
================================

## Lista de conteúdo:

1) [Informações](#informações)
2) [Instalação](#instalação)
3) [Utilização](#utilização)
4) [Contatos](#contatos)


## Informações

O POSITRON foi implementado e testado com base na versão 3.35 do [NS-3](https://www.nsnam.org), em um ambiente Linux Ubuntu 20.04.

Para o seu funcionamento, é necessária a instalação prévia do SQLite 3 e da biblioteca YAML CPP.

Instruções de instalação e uso são descritas a seguir.


## Instalação

Primeiramente, deve-se fazer um clone deste repositório através da ferramenta Git

```shell
git clone https://github.com/ifpb/positron
```

Após a clonagem do repositório, recomenda-se a instalação das dependências

```shell
cd positron 
sudo bash scripts/dependencies.sh
```

A instalação deve continuar conforme os passos do próprio NS-3, apenas com a indicação da biblioteca YAML como diferença

```shell
CXFLAGS_EXTRA="-I/usr/include/yaml-cpp" LDFLAGS="-lyaml-cpp" ./waf configure --enable-examples --enable-tests
```

seguido de

```shell
./waf build
```

## Utilização

Uma vez compilado, pode-se executar o POSITRON a partir dos arquivos main.cc e input.yaml no diretório scracth.

```shell
./waf --run main 
```

Basta alterar o arquivo input.yaml para configurar novos cenários de simulação.

Exemplos de parâmetros de simulação

```shell
./waf --run "main --seed=79" 
```

```shell
./waf --run "main --logging=true --tracing=true"
```

```shell
./waf --run "main --balanced=true"
```

## Contatos

* Aldri Santos (aldri[at]dcc.ufmg.br)
* Ayrton M. Porto de Gois (ayrton.porto[at]academico.ifpb.edu.br)
* Diego M. Rocha (diego.rocha[at]academico.ifpb.edu.br)
* Fernando Matos (fernando[at]ci.ufpb.br)
* Jefferson Lucas Ferreira da Silva (ferreira.jefferson[at]academico.ifpb.edu.br)
* Leandro H. Batista da silva (leandro.batista[at]academico.ifpb.edu.br)
* Lucas Carvalho Goncalves Silva (lucas.goncalves[at]academico.ifpb.edu.br)
* Paulo Ditarso Maciel Jr. (paulo.maciel[at]ifpb.edu.br)
* Ricardo Pereira Lins (ricardo.lins[at]academico.ifpb.edu.br)

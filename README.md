<!-- markdownlint-disable -->
<div align="center">

# Socks

![socks_logo](./socks.png)

Sistema simples de transferêcnia de arquivos com raw sockets

[![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/built-with-grammas-recipe.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/60-percent-of-the-time-works-every-time.svg)](https://forthebadge.com)

</div>
<!-- markdownlint-restore -->

## 📑 Sumário
- [📁 Estrutura de Pastas](#-estrutura-de-pastas)
- [🛠 Configuração](#-configuração)
- [🔨 Compilação](#-compilação)
- [🚀 Execução](#-execução)

## 📁 Estrutura de Pastas
```
rawSocket/
├── include/
│   └── common/
│       ├── socket_utils.hpp
│       └── file_utils.hpp
├── src/
│   ├── common/
│   │   ├── socket_utils.cpp
│   │   └── file_utils.cpp
│   ├── sender/
│   │   └── sender.cpp
│   └── receiver/
│       └── receiver.cpp
├── docs/
│   ├── protocol.md
│   ├── raw_sockets.md
│   └── architecture.md
└── Makefile
```

## 🛠 Configuração

### Dependências
Para compilar e executar o projeto, você precisará das seguintes ferramentas:

#### Essenciais
- `g++` (versão 17 ou superior)
- `make`
- `cmake`

Instalação em sistemas baseados em Debian/Ubuntu:
```bash
sudo apt update
sudo apt install g++ make cmake
```

## 🔨 Compilação

Para compilar o projeto, crie uma pasta `build` e execute o `cmake`:

```bash
mkdir build
cd build
```

```bash
cmake ..
```

E então compile o projeto com o `make`:

```bash
make
```

Isso irá gerar dois executáveis:
- `sender`: Programa para enviar arquivos
- `receiver`: Programa para receber arquivos

## 🚀 Execução

O sistema requer privilégios de root para operar raw sockets.

1. Inicie o receiver:
```bash
sudo ./receiver <ip> <porta> <arquivo_saida>
```

2. Em outro terminal, execute o sender:
```bash
sudo ./sender <ip> <porta> <arquivo>
```

Onde:
- `<ip>`: Endereço IP do receiver (ex: 127.0.0.1)
- `<porta>`: Porta para comunicação (ex: 8080)
- `<arquivo>`: Caminho do arquivo a ser enviado
- `<arquivo_saida>`: Caminho do arquivo de saída no receiver

Exemplos:
```bash
sudo ./receiver 127.0.0.1 8080 output.txt
```

```bash
sudo ./sender 127.0.0.1 8080 input.txt
```

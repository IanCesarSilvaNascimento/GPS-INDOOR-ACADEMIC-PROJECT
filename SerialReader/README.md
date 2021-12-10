# PythonSerialReader

Isso é um experimento que foi utilizado para ler dados escritos na porta serial, e persistilos em banco de dados.

Este projeto foi feito especificamente para um projeto Arduino onde atráves da triangulação de pontos determinasse a posição de determinado objeto no espaço.

O projeto verifica quantas vezes o objeto passou por aquele ponto, pois a premissa do mesmo é garantir ao usuário a informação sobre quantas vezes aquele objeto passa por aquele ponto durante um determinado período.

Porém pode ser facilmente adaptado para diversas configurações utilizando os mesmos conceitos aqui utilizados.

## Configurando Ambiente

Este programa foi escrito em Python 2.7.17

Para o python é necessário a utilização da biblioteca Serial,  ela garante métodos já prontos para executar a tarefa de leitura e escrita na porta serial, como diversas outras manipulaões e configurações.

Use o package manager [pip](https://pip.pypa.io/en/stable/) para sqlalchemy and PySerial.

```bash
pip install sqlalchemy serial
```

Nesta aplicação foi utilizado o banco de dados postgresql, porém com a utilização do orm sqlalchemy grande parte dos bancos mais utilizados podem ser utilizados igualmente.

Toda informação de conexão com banco deve ser posta em um arquivo de configuração, e ela que determina o banco de dados a ser usado como no exemplo:

```Code
DATABASE_URI = 'postgres+psycopg2://user:password@localhost:port/database'
```

o DbContext fará o trabalho de conectar ao seu banco de dados de preferência.

Cada banco de dados utilizará um módulo diferente, neste exemplo utilizei o psycopg2, para instala-lo simplesmente use o package manager mais uma vez:
```bash
pip install psycopg2
```


## Utilização
Para utilizar a aplicação simplesmente use comando:

```python
python SerialReader.py
```

## Contribuição

Dicas sobre organização de código e melhoras de perfomance são bem vindas, porém pull request's não serão analisados, o objetivo deste projeto é ser um modelo para ser usado em futuros projetos ou fork's deste mesmo projeto.

## License
[MIT](https://choosealicense.com/licenses/mit/)
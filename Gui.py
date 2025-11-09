from flask import Flask, render_template, url_for , redirect , request, session
import requests
import json
from ctypes import *
import pdb

#Declara as variaveis que vamos passar para o código de C como Nulas
cabeca = None
ultimoBlcCriado = None

#Define a Struct de bloco idêntica a sua contraparte, para podermos utilizar as funções do código em C
class bloco(Structure):
    _fields_ = [
        ("codigo", c_char_p * 20),
        ("estagio", c_char_p * 200),
        ("hashAnterior", POINTER(c_char_p)),
        ("dataHoraC", c_char_p * 26),
        ("hashAtual", c_char_p * 76),
    ]

#Define a Struct de node idêntica a sua contraparte, para podermos utilizar as funções do código em C
class node(Structure):
    pass
node._fields_ = [
        ("codigo", c_char_p * 20),
        ("vetor", c_int),
        ("blc", POINTER(bloco)),
        ("link", POINTER(node))
    ]

#Carrega o código em c e define o que cada função deve retornar (EX:"abrirArquivo" retorna um ponteiro para um node)
libObject = cdll.LoadLibrary('./blockchain.so')
libObject.passarLista.restype = c_char_p
libObject.abrirArquivo.restype = POINTER(node)
libObject.entrada.restype = POINTER(node)


def BuscaLista(entrada):
    #Função que traduz sua entrada para ser compativel com a linguagem C
    #Depois traduz o retorno da função em C para uma String que o Python consiga utilizar.  
    #Utilizada para buscar uma cadeia a partir de um código.
    global cabeca
    
    cabeca = libObject.abrirArquivo(entrada.encode("utf-8"),cabeca)
    lista = libObject.passarLista(cabeca)
    return lista.decode('utf-8')

def adcBlc(entrada):
    #Função que traduz sua entrada para ser compativel com a linguagem C
    #Utilizada para criar um bloco a partir da entrada do usuário
    global cabeca
    cabeca = libObject.entrada(cabeca,entrada.encode("utf-8"))


#Define o objeto utilizado para inicializar um WebServer pelo Flask.
app = Flask(__name__)
app.config['SECRET KEY'] = '5791628bb0b13ce0c676dfde280ba245'


@app.route("/")
def index():
    #Redireciona o usuário para a página principal assim que ele acessa o IP do WebServer
    return redirect(url_for("formulario"))
    
@app.route("/formulario",methods=["POST","GET"])
def formulario():
    #Página principal do programa, aqui o usuário é apresentado com a opção de buscar uma cadeia ou iniciar uma cadeia.

    #Garante que toda vez que a página é acessada a memória do programa será limpa, para não haver conflitos.
    global cabeca
    libObject.limpaLista(cabeca)
    cabeca = None

    #Checa se a pagina foi acessada com um método de "POST", significando que o usuário enviou um formulário e deve ser
    #Redirecionado para outra página com os resultados.
    if request.method == "POST":
        #Coleta informações de qual formulário foi enviado.
        chave = (list(request.form.keys())[0])
        if chave == "proc":
            #Caso o formulário de procura de cadeia tenha sido enviado, pega suas informações,
            #Chama a função de busca de cadeia e retorna todos os blocos da lista em formato de string 
            #E redireciona para a página de resultados com todos os blocos ou retorna um código de erro.

            codigo = request.form["proc"]
            
            dic = BuscaLista(codigo)

            if dic == "Codigo inexistente!":
                print("Código inexistente!")
                return render_template('formErr.html')
            else:
                    return redirect(url_for('result', resultado = str(dic)))
        elif chave  == "cria" :
            #Caso o formulário de criação de cadeia tenha sido enviado, pega suas informações,
            #Chama a função de criação de cadeia com a entrada do usuário e redireciona o usuário
            #Para a página de resultados, mostrando o bloco que o mesmo acabou de criar.
            nome = request.form["cria"]
            
            adcBlc(nome)
            lista = libObject.passarLista(cabeca)
            print(lista)
            return redirect(url_for('result', resultado = str(lista.decode('utf-8'))))
        else:
            #Caso um formulário não tenha sido enviado, apenas retorna para a pagina principal.
            return render_template('form.html')
    else:
        #Caso um formulário não tenha sido enviado,renderiza a pagina principal.
        return render_template('form.html')
    

@app.route("/<resultado>",methods=["POST","GET"])
def result(resultado):
    #Recebe todos os blocos em formato de uma string continua, que é separada por ";" representando cada bloco e
    # "." representando as informações do bloco
    #Ex: bloco1.hash.hashAnterior;bloco2.hash.hashAnterior;...

    #Separa os blocos e os coloca em uma lista
    dic = resultado.split(';')
    #Declara um dicionario para uso futuro.
    dicionario = {}

    if request.method == "POST":
        #Caso o formulário de criação de cadeia tenha sido enviado, cria um bloco
        #E continua o processamento padrão da página, substituindo a entrada antiga
        #Por uma com o bloco criado.
        adcBlc(request.form["add"])
        temp = libObject.passarLista(cabeca)
        print(temp)
        resultadoNovo = str(temp.decode('utf-8'))
        dic = resultadoNovo.split(';')


    for i in range(len(dic) - 1):
        #Separa todas as informações de um bloco e as adiciona em uma entrada do dicionario.
        bloco = dic[i].split(".")
        if i == 0:
            #Caso seja a primeira instancia, separa o código e a validez da cadeia, já que essas informações apenas
            #Aparecem antes do primeiro bloco.
            dicionario["Codigo"] = bloco[0]
            dicionario["Validez"] = bloco[1]
            dicionario["Bloco"+str(i)] = {
                "Vetor": bloco[2],
                "Estagio": bloco[3],
                "Data e Hora": bloco[4],
                "Hash Anterior": bloco[5],
                "Hash Atual": bloco[6]
            }
        else:
            #Caso não seja a primeira entrada, apenas segue formando o dicionario de blocos.
            dicionario["Bloco"+str(i)] = {
                "Vetor": bloco[0],
                "Estagio": bloco[1],
                "Data e Hora": bloco[2],
                "Hash Anterior": bloco[3],
                "Hash Atual": bloco[4]
            }
    #Separa as informações de código e validez para poderem ser usadas separadamente na página
    cod = dicionario["Codigo"]
    validez = dicionario["Validez"]
    #Deleta as entradas de código e validez do dicionario para não haver conflitos
    del dicionario["Codigo"]
    del dicionario["Validez"]

    #Chama a página de resultados.
    return render_template('result.html', data = dicionario, validez=int(validez),codigo=str(cod))


#Incia o WebServer.
if __name__ == "__main__":
    app.run()


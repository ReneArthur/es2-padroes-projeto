# Atividade de Engenharia de Software II
Nome: Rene Arthur Rocha de Souza.
Linguagem escolhida: C++

**Sumário**
- [Sobre o projeto](#Sobre-o-projeto)
- [Padrão Criacional: Builder](#Padrao-Criacional-Builder)

## Sobre o projeto
O projeto é sobre criação de personagens, de RPG de mesa, de nível 1, utilizando as regras do livro Tormenta20.
A classe que armazena o personagem, seus atributos, habilidades, vida e mana é Personagem:

```

class Personagem {
    public:
        Personagem() {}
        void setAtributos(int forc, int dest, 
        int cons, int inte, int sabe, int cari) {
            this->forca = forc;
            this->destreza = dest;
            this->constituicao = cons;
            this->inteligencia = inte;
            this->sabedoria = sabe;
            this->carisma = cari;
        };
        void addHabilidade(Habilidade hab) {
            habilidades.push_back(hab);
        };

        string nome;

        int forca;
        int destreza;
        int constituicao;
        int inteligencia;
        int sabedoria;
        int carisma;
        
        int vida;
        int mana;
    private:
        vector<Habilidade> habilidades;  
};

```
> Nota: nesse exemplo de código eu tirei as funções: imprimir, getModificacador, getModificadorForca e etc..., para mostrar apenas os atributos da classe.

E essa é a classe que é usada pelo *Personagem*, a classe *Habilidade* 

```
class Habilidade {
    public:
        Habilidade(string nome, string descricacao) {
            this->nome = nome;
            this->descricao = descricacao;
        }
        string nome;
        string descricao;
};
```

## Padrão Criacional: Builder

Há muitos jeitos diferentes de criar um personagem, há 17 raças e 14 profissões (ou também chamado de classe), cada um com suas vantagens e disvantagens únicas, ou seja, apenas para o nível 1, há 238 combinações diferentes de *Personagem* (isso sem contar os atributos que o jogador escolhe o número). Por isso, o padrão criacional é a melhor opção para criar um *Personagem*, pois vai diminuir muito o código e vai ser possível reutilizá-lo.


### Aplicando no código

A ideia é separar as raças em classes construtoras que herdam de *Construtor*, por exemplo: *ConstrutorHumano*, *ConstrutorElfo*.

#### Construtores

Cada raça tem bônus e prejuízo diferente para os atributos base (atributos em RPG é força, destreza, constituição, inteligência, sabedoria e carisma), e cada raça também tem habilidades diferentes.

Logo a classe abstrata Construtor, vai ter os métodos abstratos **produzirAtributos** e **produzirHabilidadesRaca**.
E também ele possui os seguintes métodos:
- **adicionarHabilidadeExtra**: esse método serve para possibilitar a adição de novas habilidades, afinal a profissão escolhida no jogo vem com algumas habilidades também.
- **getPersonagem**: para conseguir uma cópia do personagem, sim ele clona o objeto *Personagem*, por enquanto não estou trabalhando com pointers por simplificação, por isso essa abordagem.
- **Construtor**: é o construtor do *Construtor*, ele recebe o nome do personagem e seus atributos base e adiciona eles no objeto Personagem.

```
class Construtor {
    public:
        Construtor(string nome, int forc, int 
        dest, int cons, int inte, int sabe, int cari) {
            personagem.setAtributos(forc, dest, 
            cons, inte, sabe, cari);
            personagem.nome = nome;
        };
        virtual void produzirAtributos() = 0;
        virtual void produzirHabilidadesRaca() = 0;
        void adicionarHabilidadeExtra(Habilidade hab) {
            personagem.addHabilidade(hab);
        };
        Personagem getPersonagem() {
            return personagem;
        };
        Personagem personagem;
};
```

*ConstrutorHumano*

ConstrutorHumano sobrescreve os métodos abstratos para aplicar suas regras de vantagem e desvantagem da raça.

```
class ConstrutorHumano: public Construtor {
    public:
        //puxando o construtor do Construtor
        using Construtor::Construtor;

        void produzirAtributos() override {
            personagem.forca += 2;
            personagem.destreza += 2;
            personagem.constituicao += 2;
        };
        void produzirHabilidadesRaca() override {
            personagem.addHabilidade(
                Habilidade("Versáril", "Você se 
                torna treinado em duas perícias 
                a sua escolha")
            );
        };
};
```

*ConstrutorElfo*

ConstrutorElfo sobrescreve os métodos abstratos para aplicar suas regras de vantagem e desvantagem da raça.

```
class ConstrutorElfo: public Construtor {
    public:
        //puxando o construtor do Construtor
        using Construtor::Construtor;

        void produzirAtributos() override {
            personagem.inteligencia += 4;
            personagem.destreza += 2;
            personagem.constituicao -= 2;
        };
        void produzirHabilidadesRaca() override {
            personagem.addHabilidade(
                Habilidade("Graça de Glórienn", 
                "Seu deslocamento é 12m (em vez 
                de 9m)")
            );
            personagem.addHabilidade(
                Habilidade("Herança Feérica", 
                "Você recebe +1 ponto de mana 
                por nível.")
            );
            personagem.addHabilidade(
                Habilidade("Sentidos Élficos", 
                "recebe Visão penumbra e +2 em 
                misticismo e percepção")
            );
        };
};
```

#### Diretor

É na classe *Diretor* que ficará as regras de criação das profissões dos personagens. 
O diretor vai ter as profissões Cavaleiro e Ladino.
A diferença entre profissões, é as habilidades extras (além da habilidade de raça) e a quantidade de vida e mana.

```
class Diretor {
    public:
        void construirCavaleiro(Construtor& builder) {
            builder.produzirAtributos();
            builder.produzirHabilidadesRaca();
            builder.adicionarHabilidadeExtra(
                Habilidade("Código de honra", 
                "Você não pode atacar um inimigo 
                pelas costas")
            );
            builder.adicionarHabilidadeExtra(
                Habilidade("Baluarte", "1PM para 
                +2 defesa e resistência.")
            );
            builder.personagem.vida = 20 + builder.personagem.getModificacaoConstituicao(); 
            builder.personagem.mana = 3;
        }
        void construirLadino(Construtor& builder) {
            builder.produzirAtributos();
            builder.produzirHabilidadesRaca();
            builder.adicionarHabilidadeExtra(
                Habilidade("Ataque furtivo", 
                "1d6 de dano adicional ao ataque 
                furtivo")
            );
            builder.adicionarHabilidadeExtra(
                Habilidade("Especialista", 
                "Escolha um (inteligência mínimo 
                1) de perícias, você pode gastar 
                1PM para dobrar seu bônus de 
                treinamento, pode usar em testes 
                de ataque")
            );
            builder.personagem.vida = 12 + builder.personagem.getModificacaoConstituicao(); 
            builder.personagem.mana = 4;
        }
};
```
> Eu não mostrei o método getModificacaoConstituicao, mas ele é um método da classe Personagem que faz um cálculo para descobrir o "modificador" da constituição.
---
#### Exemplo de criação de personagem

```
int main() {
    ConstrutorHumano cc1("Vander", 15, 10, 18, 10, 12, 12);
    Diretor d1;
    d1.construirCavaleiro(cc1);

    Personagem p1 = cc1.getPersonagem();
    p1.imprimir();

    return 0;
}
```

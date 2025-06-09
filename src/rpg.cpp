#include <iostream>
#include <vector>
#include <cmath>
#include <set>

// enum Atributos {
//     FORCA,
//     DESTREZA,
//     CONSTITUICAO,
//     INTELIGENCIA,
//     SABEDORIA,
//     CARISMA
// };

// struct {
//     int forca;
//     int destreza;
//     int constituicao;
//     int inteligencia;
//     int sabedoria;
//     int carisma;
// } Atributos2;


class Habilidade {
    public:
        Habilidade(std::string nome, std::string descricacao) {
            this->nome = nome;
            this->descricao = descricacao;
        }
        std::string nome;
        std::string descricao;

        /*
            Operação usada pelo SET, para calcular a ordem para por os elementos de Habilidade
        */
        bool operator<(const Habilidade& hab) const {
            return nome < hab.nome;
        }
};

//flyweight, a única diferença é que eu n tenho o repeating state
//apenas o unique state, por isso vou usar um (set) em vez de (map)
class FlyweightFactoryHabilidade {
    public:
        const Habilidade* getFlyweight(const Habilidade& habilidade) {
            /*
                (insert) insere habilidade, se já existe apenas retorna a 
                nova alocação dentro do campo (.first)
            */
            auto it = cache.insert(habilidade);

            // convertendo first (que é do tipo iterator) para o tipo pointer
            return &(*it.first);
        };
    private: 
        std::set<Habilidade> cache;
};

FlyweightFactoryHabilidade ffh;

class Personagem {
    public:
        Personagem(): ffh(::ffh) {}
        void imprimir() {
            std::cout << "--------\n";
            std::cout << "Nome: " << nome << "\n"
            << "raça: " << raca << "\n"
            << "vida: " << vida << " | mana: " << mana << "\n\n"
            << "ATRIBUTOS:\n"
            << "forca: " << forca << "\n"
            << "destreza: " << destreza << "\n"
            << "constituição: " << constituicao << "\n"
            << "inteligência: " << inteligencia << "\n"
            << "sabedoria: " << sabedoria << "\n"
            << "carisma: " << carisma << "\n"
            << "----\n"
            << "Habilidades: \n";

            for(const Habilidade* hab: habilidades) {
                std::cout << "nome: " << hab->nome << "\ndescrição: " << hab->descricao << "\n\n";
            }

            std::cout << "--------\n";
        }
        void setAtributos(int forc, int dest, int cons, int inte, int sabe, int cari) {
            this->forca = forc;
            this->destreza = dest;
            this->constituicao = cons;
            this->inteligencia = inte;
            this->sabedoria = sabe;
            this->carisma = cari;
        };
        void addHabilidade(const Habilidade& hab) {
            const Habilidade* pointerHabilidade = ffh.getFlyweight(hab);

            habilidades.push_back(pointerHabilidade);

        };
        int getModificacaoForca() {
            return getModificacador(forca);
        }
        int getModificacaoDestreza() {
            return getModificacador(destreza);
        }
        int getModificacaoConstituicao() {
            return getModificacador(constituicao);
        }
        int getModificacaoInteligencia() {
            return getModificacador(inteligencia);
        }
        int getModificacaoSabedoria() {
            return getModificacador(sabedoria);
        }
        int getModificacaoCarisma() {
            return getModificacador(carisma);
        }
        std::string nome;

        std::string raca;

        int forca;
        int destreza;
        int constituicao;
        int inteligencia;
        int sabedoria;
        int carisma;
        
        int vida;
        int mana;

        FlyweightFactoryHabilidade& ffh;
    private:
        int getModificacador(int valor) {
            return floor((float)(valor - 10) / 2);
        }
        std::vector<const Habilidade*> habilidades;  
};

class Construtor {
    public:
        Construtor(std::string nome, int forc, int dest, int cons, int inte, int sabe, int cari) {
            personagem.setAtributos(forc, dest, cons, inte, sabe, cari);
            personagem.nome = nome;
        };
        virtual void setRaca() = 0;
        virtual void produzirAtributos() = 0;
        virtual void produzirHabilidadesRaca() = 0;
        void adicionarHabilidadeExtra(const Habilidade& hab) {
            personagem.addHabilidade(hab);
        };
        Personagem getPersonagem() {
            return personagem;
        };
        Personagem personagem;
};

class ConstrutorHumano: public Construtor {
    public:
        //puxando o construtor do Construtor
        using Construtor::Construtor;

        void setRaca() override {
            personagem.raca = "Humano";
        }

        void produzirAtributos() override {
            personagem.forca += 2;
            personagem.destreza += 2;
            personagem.constituicao += 2;
        };
        void produzirHabilidadesRaca() override {
            personagem.addHabilidade(
                Habilidade("Versáril", "Você se torna treinado em duas perícias a sua escolha")
            );
        };
};

class ConstrutorElfo: public Construtor {
    public:
        //puxando o construtor do Construtor
        using Construtor::Construtor;
    
        void setRaca() override {
            personagem.raca = "Elfo";
        }

        void produzirAtributos() override {
            personagem.inteligencia += 4;
            personagem.destreza += 2;
            personagem.constituicao -= 2;
        };
        void produzirHabilidadesRaca() override {
            personagem.addHabilidade(
                Habilidade("Graça de Glórienn", "Seu deslocamento é 12m (em vez de 9m)")
            );
            personagem.addHabilidade(
                Habilidade("Herança Feérica", "Você recebe +1 ponto de mana por nível.")
            );
            personagem.addHabilidade(
                Habilidade("Sentidos Élficos", "recebe Visão penumbra e +2 em misticismo e percepção")
            );
        };
};

class Diretor {
    public:
        void construirCavaleiro(Construtor& builder) {
            construirBasePersonagem(builder);

            builder.adicionarHabilidadeExtra(
                Habilidade("Código de honra", "Você não pode atacar um inimigo pelas costas")
            );
            builder.adicionarHabilidadeExtra(
                Habilidade("Baluarte", "1PM para +2 defesa e resistência.")
            );
            builder.personagem.vida = 20 + builder.personagem.getModificacaoConstituicao(); 
            builder.personagem.mana = 3;
        }
        void construirLadino(Construtor& builder) {
            construirBasePersonagem(builder);

            builder.adicionarHabilidadeExtra(
                Habilidade("Ataque furtivo", "1d6 de dano adicional ao ataque furtivo")
            );
            builder.adicionarHabilidadeExtra(
                Habilidade("Especialista", "Escolha um (inteligência mínimo 1) de perícias, você pode gastar 1PM para dobrar seu bônus de treinamento, pode usar em testes de ataque")
            );
            builder.personagem.vida = 12 + builder.personagem.getModificacaoConstituicao(); 
            builder.personagem.mana = 4;
        }
    private:
        void construirBasePersonagem(Construtor& builder) {
            builder.setRaca();
            builder.produzirAtributos();
            builder.produzirHabilidadesRaca();
        }
};

int main() {

    ConstrutorHumano cc1("Vander", 15, 10, 18, 10, 12, 12);
    Diretor d1;
    d1.construirCavaleiro(cc1);
    
    Personagem p1 = cc1.getPersonagem();
    p1.imprimir();
    
    ConstrutorElfo ce1("Estes", 10, 13, 10, 18, 17, 11);
    d1.construirLadino(ce1);
    Personagem p2 = ce1.getPersonagem();
    p2.imprimir();

    return 0;
}

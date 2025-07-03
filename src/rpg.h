#ifndef RPG
#define RPG

#include <string>
#include <vector>
#include <set>

class Personagem;
class Habilidade;

class Visitor {
    public:
    virtual void visitPersonagem(const Personagem* p) const = 0;
    virtual void visitHabilidade(const Habilidade* h) const = 0;
};

class Visitable {
    public:
    virtual void accept(Visitor& v) const = 0;
};

class Habilidade: public Visitable {
    public:
        Habilidade(std::string nome, std::string descricacao);
        std::string nome;
        std::string descricao;

        bool operator<(const Habilidade& hab) const;

        void accept(Visitor& v) const override;
};

class FlyweightFactoryHabilidade {
    public:
        const Habilidade* getFlyweight(const Habilidade& habilidade);
    private: 
        std::set<Habilidade> cache;
};

class Personagem: public Visitable {
    public:
        Personagem();
        void imprimir();
        void setAtributos(int forc, int dest, int cons, int inte, int sabe, int cari);
        void addHabilidade(const Habilidade& hab);
        int getModificacaoForca();
        int getModificacaoDestreza();

        int getModificacaoConstituicao();
        int getModificacaoInteligencia();
        int getModificacaoSabedoria();
        int getModificacaoCarisma();

        void accept(Visitor& v) const override;

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

        std::vector<const Habilidade*> habilidades;  
    private:
        int getModificacador(int valor);
};

class JSONExportVisitor: public Visitor {
    private:
        std::string formatarHabilidade(const Habilidade* hab) const;
    public:
    void visitPersonagem(const Personagem* p) const override;
    void visitHabilidade(const Habilidade* const h) const override;
};

class Construtor {
    public:
        Construtor(std::string nome, int forc, int dest, int cons, int inte, int sabe, int cari);
        virtual void setRaca() = 0;
        virtual void produzirAtributos() = 0;
        virtual void produzirHabilidadesRaca() = 0;
        void adicionarHabilidadeExtra(const Habilidade& hab);
        Personagem getPersonagem();
        Personagem personagem;
};

class ConstrutorHumano: public Construtor {
    public:
        using Construtor::Construtor;

        void setRaca() override;

        void produzirAtributos() override;
        void produzirHabilidadesRaca() override;
};

class ConstrutorElfo: public Construtor {
    public:
        using Construtor::Construtor;
    
        void setRaca() override;

        void produzirAtributos() override;
        void produzirHabilidadesRaca() override;
};

class Diretor {
    public:
        void construirCavaleiro(Construtor& builder);
        void construirLadino(Construtor& builder);
    private:
        void construirBasePersonagem(Construtor& builder);
};

#endif
#include <iostream>
#include <vector>
#include <cmath>
#include <set>
#include <fstream>
#include "rpg.h"

// class Personagem;
// class Habilidade;


Habilidade::Habilidade(std::string nome, std::string descricao) {
    this->nome = nome;
    this->descricao = descricao;
}
bool Habilidade::operator<(const Habilidade& hab) const {
    return nome < hab.nome;
}

void Habilidade::accept(Visitor& v) const {
    v.visitHabilidade(this);
}


//flyweight, a única diferença é que eu n tenho o repeating state
//apenas o unique state, por isso vou usar um (set) em vez de (map)
const Habilidade* FlyweightFactoryHabilidade::getFlyweight(const Habilidade& habilidade) {
    /*
        (insert) insere habilidade, se já existe apenas retorna a 
        nova alocação dentro do campo (.first)
    */
    auto it = cache.insert(habilidade);

    // convertendo first (que é do tipo iterator) para o tipo pointer
    return &(*it.first);
};

FlyweightFactoryHabilidade ffh;

Personagem::Personagem(): ffh(::ffh) {}

void Personagem::imprimir() {
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
void Personagem::setAtributos(int forc, int dest, int cons, int inte, int sabe, int cari) {
    this->forca = forc;
    this->destreza = dest;
    this->constituicao = cons;
    this->inteligencia = inte;
    this->sabedoria = sabe;
    this->carisma = cari;
};
void Personagem::addHabilidade(const Habilidade& hab) {
    const Habilidade* pointerHabilidade = ffh.getFlyweight(hab);

    habilidades.push_back(pointerHabilidade);

};
int Personagem::getModificacaoForca() {
    return getModificacador(forca);
}
int Personagem::getModificacaoDestreza() {
    return getModificacador(destreza);
}
int Personagem::getModificacaoConstituicao() {
    return getModificacador(constituicao);
}
int Personagem::getModificacaoInteligencia() {
    return getModificacador(inteligencia);
}
int Personagem::getModificacaoSabedoria() {
    return getModificacador(sabedoria);
}
int Personagem::getModificacaoCarisma() {
    return getModificacador(carisma);
}

void Personagem::accept(Visitor& v) const {
    v.visitPersonagem(this);
}

int Personagem::getModificacador(int valor) {
    return floor((float)(valor - 10) / 2);
}

std::string JSONExportVisitor::formatarHabilidade(const Habilidade* hab) const {
    return "{\"nome\": \"" + hab->nome + "\",\"descricao\": \"" + hab->descricao + "\"}"; 
}
void JSONExportVisitor::visitPersonagem(const Personagem* p) const {
    std::ofstream arquivo("personagem.json");

    arquivo << "{";

    arquivo << 
    "\"nome\": \"" << p->nome << "\", "
    "\"raca\": \"" << p->raca << "\", "
    "\"atributos\": {"
        "\"forca\": " << p->forca << ", "
        "\"destreza\": " << p->destreza << ", "
        "\"constituicao\": " << p->constituicao << ", "
        "\"inteligencia\": " << p->inteligencia << ", "
        "\"sabedoria\": " << p->sabedoria << ", "
        "\"carisma\": " << p->carisma
    << "},"
    "\"vida\": \"" << p->vida << "\", " 
    "\"mana\": \"" <<  p->mana << "\", "
    ;

    arquivo << "\"habilidades\": [";

    for(auto habilidade: p->habilidades) {
        
        arquivo << formatarHabilidade(habilidade);

        if(habilidade != p->habilidades.back()) {
            arquivo << ",";
        }
    }
    arquivo << "]";

    arquivo << "}";
    arquivo.close();
}
void JSONExportVisitor::visitHabilidade(const Habilidade* const h) const {
    std::ofstream arquivo("habilidade.json");
    arquivo << formatarHabilidade(h);
    arquivo.close();
}

Construtor::Construtor(std::string nome, int forc, int dest, int cons, int inte, int sabe, int cari) {
    personagem.setAtributos(forc, dest, cons, inte, sabe, cari);
    personagem.nome = nome;
};
void Construtor::adicionarHabilidadeExtra(const Habilidade& hab) {
    personagem.addHabilidade(hab);
};
Personagem Construtor::getPersonagem() {
    return personagem;
};


void ConstrutorHumano::setRaca() {
    personagem.raca = "Humano";
}

void ConstrutorHumano::produzirAtributos() {
    personagem.forca += 2;
    personagem.destreza += 2;
    personagem.constituicao += 2;
};
void ConstrutorHumano::produzirHabilidadesRaca() {
    personagem.addHabilidade(
        Habilidade("Versáril", "Você se torna treinado em duas perícias a sua escolha")
    );
};


void ConstrutorElfo::setRaca() {
    personagem.raca = "Elfo";
}

void ConstrutorElfo::produzirAtributos() {
    personagem.inteligencia += 4;
    personagem.destreza += 2;
    personagem.constituicao -= 2;
};
void ConstrutorElfo::produzirHabilidadesRaca() {
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

void Diretor::construirCavaleiro(Construtor& builder) {
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
void Diretor::construirLadino(Construtor& builder) {
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
void Diretor::construirBasePersonagem(Construtor& builder) {
    builder.setRaca();
    builder.produzirAtributos();
    builder.produzirHabilidadesRaca();
}


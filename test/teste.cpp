#include <catch2/catch_test_macros.hpp>
#include "rpg.h"

TEST_CASE("RPG - humano cavaleiro com vida e mana certa", "[Humano][Cavaleiro][vida][mana]") {
    int vidaCerta = 25;
    int manaCerta = 3;

    ConstrutorHumano cc1("Vander", 15, 10, 18, 10, 12, 12);
    Diretor d1;
    d1.construirCavaleiro(cc1);
    Personagem p1 = cc1.getPersonagem();
    
    REQUIRE(p1.vida == vidaCerta);
    REQUIRE(p1.mana == manaCerta);
}

TEST_CASE("RPG - humano ladino com vida e mana certa", "[Humano][Ladino][vida][mana]") {
    int vidaCerta = 17;
    int manaCerta = 4;

    ConstrutorHumano cc1("Vander", 15, 10, 18, 10, 12, 12);
    Diretor d1;
    d1.construirLadino(cc1);
    Personagem p1 = cc1.getPersonagem();
    
    REQUIRE(p1.vida == vidaCerta);
    REQUIRE(p1.mana == manaCerta);
}

TEST_CASE("RPG - elfo cavaleiro com vida e mana certa", "[Elfo][Cavaleiro][vida][mana]") {
    int vidaCerta = 23;
    int manaCerta = 3;

    ConstrutorElfo cc1("Estes", 15, 10, 18, 10, 12, 12);
    Diretor d1;
    d1.construirCavaleiro(cc1);
    Personagem p1 = cc1.getPersonagem();
    
    REQUIRE(p1.vida == vidaCerta);
    REQUIRE(p1.mana == manaCerta);
}

TEST_CASE("RPG - elfo ladino com vida e mana certa", "[Elfo][Ladino][vida][mana]") {
    int vidaCavaleiro = 15;
    int manaCavaleiro = 4;

    ConstrutorElfo cc1("Estes", 15, 10, 18, 10, 12, 12);
    Diretor d1;
    d1.construirLadino(cc1);
    Personagem p1 = cc1.getPersonagem();
    
    REQUIRE(p1.vida == vidaCavaleiro);
    REQUIRE(p1.mana == manaCavaleiro);
}

TEST_CASE("RPG - Cache de habilidades deve funcionar", "[Habilidades][Cache][vida][mana]") {
    std::string descricao = "habilidade1";
    
    Personagem p1;
    p1.addHabilidade(Habilidade("Teste", descricao));
    p1.addHabilidade(Habilidade("Teste2", "segunda habilidade"));
    
    Personagem p2;
    p2.addHabilidade(Habilidade("Teste", "Essa descricao deve sumir pois já tem uma habilidade teste"));

    // o pointer da habilidade do p1 tem que ser o mesmo do p2 
    REQUIRE(p1.habilidades[0] == p2.habilidades[0]);

    // o pointer da segunda habilidade do p1 tem que ser diferente da primeira
    REQUIRE(p1.habilidades[1] != p2.habilidades[0]);
    
    // A descrição da primeira habilidade do p2 tem que ser a mesma descrição usada no p1
    REQUIRE(p2.habilidades[0]->descricao == descricao);

    // No final criamos 2 habilidades para o p1 e 1 para o p2
    REQUIRE(p1.habilidades.size() == 2);
    REQUIRE(p2.habilidades.size() == 1);
}


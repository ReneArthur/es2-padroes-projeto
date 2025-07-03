#include <iostream>
#include "rpg.h"

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

    JSONExportVisitor jev;
    p2.accept(jev);
    p2.habilidades.at(0)->accept(jev);

    return 0;
}

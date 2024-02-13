#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

enum class ValorCarta {
    As = 1, Dos, Tres, Cuatro, Cinco, Seis, Siete, Ocho, Nueve, Diez, Jota, Reina, Rey
};

enum class PaloCarta {
    Corazones,
    Picas,
    Rombos,
    Treboles
};

struct Carta {
    ValorCarta valor;
    PaloCarta palo;
};

int CalcularValorMano(const std::vector<Carta>& mano, bool esJugador) {
    int valorTotal = 0;
    int numAces = 0;

    for (const Carta& carta : mano) {
        int valorCarta = static_cast<int>(carta.valor);
        if (valorCarta == 1) {
            numAces++;
            valorTotal += 11; // As vale inicialmente 11
        }
        else {
            valorTotal += (valorCarta > 10) ? 10 : valorCarta;
        }
    }

    // Tratar el As como 1 si excede 21
    while (valorTotal > 21 && numAces > 0) {
        valorTotal -= 10; // Restar 10 al valor total
        numAces--; // Reducir el número de As tratados como 11
    }

    return valorTotal;
}


bool SePasaDe21(const std::vector<Carta>& mano) {
    return CalcularValorMano(mano, false) > 21;
}

bool Tiene21(const std::vector<Carta>& mano) {
    return CalcularValorMano(mano, false) == 21;
}

bool HaGanado(const std::vector<Carta>& manoJugador, const std::vector<Carta>& manoCroupier) {
    int valorManoJugador = CalcularValorMano(manoJugador, true);
    int valorManoCroupier = CalcularValorMano(manoCroupier, false);

    return (Tiene21(manoJugador) && !Tiene21(manoCroupier)) ||
        (valorManoJugador <= 21 && (valorManoCroupier > 21 || valorManoJugador > valorManoCroupier)) ||
        (Tiene21(manoJugador) && Tiene21(manoCroupier));
}

bool HaPerdido(const std::vector<Carta>& manoJugador, const std::vector<Carta>& manoCroupier) {
    int valorManoJugador = CalcularValorMano(manoJugador, true);
    int valorManoCroupier = CalcularValorMano(manoCroupier, false);

    return SePasaDe21(manoJugador) || (valorManoJugador < valorManoCroupier && !SePasaDe21(manoCroupier));
}

void MostrarCarta(const Carta& carta) {
    switch (carta.valor) {
    case ValorCarta::As:
        std::cout << "As";
        break;
    case ValorCarta::Jota:
        std::cout << "J";
        break;
    case ValorCarta::Reina:
        std::cout << "Q";
        break;
    case ValorCarta::Rey:
        std::cout << "K";
        break;
    default:
        std::cout << static_cast<int>(carta.valor);
        break;
    }

    switch (carta.palo) {
    case PaloCarta::Corazones:
        std::cout << " Corazones";
        break;
    case PaloCarta::Picas:
        std::cout << " Picas";
        break;
    case PaloCarta::Rombos:
        std::cout << " Rombos";
        break;
    case PaloCarta::Treboles:
        std::cout << " Treboles";
        break;
    }
}

Carta RepartirCartaAleatoria() {
    static const ValorCarta valores[] = {
        ValorCarta::Dos, ValorCarta::Tres, ValorCarta::Cuatro, ValorCarta::Cinco,
        ValorCarta::Seis, ValorCarta::Siete, ValorCarta::Ocho, ValorCarta::Nueve,
        ValorCarta::Diez, ValorCarta::Jota, ValorCarta::Reina, ValorCarta::Rey, ValorCarta::As
    };

    static const PaloCarta palos[] = {
        PaloCarta::Corazones,
        PaloCarta::Picas,
        PaloCarta::Rombos,
        PaloCarta::Treboles
    };

    int indiceValor = rand() % 13;
    int indicePalo = rand() % 4;

    return { valores[indiceValor], palos[indicePalo] };
}

bool JuegoContinua(bool esJugador, int valorMano) {
    if (esJugador) {
        // Si es el jugador, seguir pidiendo cartas solo si el valor de la mano es menor o igual a 21
        return (valorMano <= 21);
    }
    else {
        // El croupier pedirá otra carta si su valor es menor que 17
        return (valorMano < 17);
    }
}

void MostrarManoCroupier(const std::vector<Carta>& manoCroupier) {
    std::cout << "Las cartas del croupier son: ";
    for (size_t i = 0; i < manoCroupier.size(); ++i) {
        if (i == 0) {
            std::cout << "Carta oculta ";
        }
        else {
            MostrarCarta(manoCroupier[i]);
        }
        std::cout << " ";
    }
    std::cout << "\n";
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    // Bucle principal
    while (true) {
        std::vector<Carta> manoJugador;
        std::vector<Carta> manoCroupier;

        manoJugador.push_back(RepartirCartaAleatoria());
        manoCroupier.push_back(RepartirCartaAleatoria());
        manoJugador.push_back(RepartirCartaAleatoria());
        manoCroupier.push_back(RepartirCartaAleatoria());

        std::cout << "Tus cartas son: ";
        for (const Carta& carta : manoJugador) {
            MostrarCarta(carta);
            std::cout << " ";
        }
        std::cout << "\n";

        MostrarManoCroupier(manoCroupier);

        int valorManoCroupier = CalcularValorMano(manoCroupier, false);
        int valorManoJugador = CalcularValorMano(manoJugador, true);

        // Bucle del jugador
// Bucle del jugador
        while (JuegoContinua(true, valorManoJugador)) {
            std::cout << "El valor de tu mano es: " << valorManoJugador << "\n";
            std::cout << "Quieres pedir otra carta? (s/n): ";
            char eleccion;
            std::cin >> eleccion;

            if (eleccion == 's' || eleccion == 'S' || eleccion == 'Si' || eleccion == 'si') {
                Carta nuevaCarta = RepartirCartaAleatoria();
                manoJugador.push_back(nuevaCarta);
                valorManoJugador = CalcularValorMano(manoJugador, true); // Recalcular el valor de la mano

                std::cout << "Has recibido la siguiente carta: ";
                MostrarCarta(nuevaCarta);
                std::cout << "\n";

                std::cout << "Tus cartas son: ";
                for (const Carta& carta : manoJugador) {
                    MostrarCarta(carta);
                    std::cout << " ";
                }
                std::cout << "\n";

                std::cout << "La primera carta del croupier es: ";
                MostrarCarta(manoCroupier[0]);
                std::cout << "\n";
            }
            else {
                break;  // Salir del bucle si el jugador no quiere más cartas
            }
        }


        // Bucle del croupier
        while (JuegoContinua(false, valorManoCroupier)) {
            // Pide carta solo si el valor de la mano del croupier es menor que 17
            if (valorManoCroupier < 17) {
                Carta nuevaCartaCroupier = RepartirCartaAleatoria();
                manoCroupier.push_back(nuevaCartaCroupier);
                valorManoCroupier = CalcularValorMano(manoCroupier, false);

                std::cout << "El croupier ha recibido la siguiente carta: ";
                MostrarCarta(nuevaCartaCroupier);
                std::cout << "\n";
            }
            else {
                break;  // Salir del bucle si el croupier no necesita más cartas
            }
        }

        // Mostrar manos y determinar el resultado
        std::cout << "Tus cartas son: ";
        for (const Carta& carta : manoJugador) {
            MostrarCarta(carta);
            std::cout << " ";
        }
        std::cout << "\n";

        std::cout << "Las cartas del croupier son: ";
        for (const Carta& carta : manoCroupier) {
            MostrarCarta(carta);
            std::cout << " ";
        }
        std::cout << "El valor de las cartas del croupier es: " << valorManoCroupier << "\n";


        if (HaGanado(manoJugador, manoCroupier)) {
            std::cout << "Has ganado.\n";
        }
        else if (HaPerdido(manoJugador, manoCroupier)) {
            std::cout << "Has perdido.\n";
        }
        else {
            std::cout << "Empate, ambos tienen el mismo valor.\n";
        }

        // Preguntar al usuario si desea jugar de nuevo
        std::cout << "Quieres jugar de nuevo? (s/n): ";
        char jugarDeNuevo;
        std::cin >> jugarDeNuevo;
        if (jugarDeNuevo != 's' && jugarDeNuevo != 'S') {
            break;
        }
    }

    return 0;
}

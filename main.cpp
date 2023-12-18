#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

class Pessoa {
public:
    Pessoa(string nome)
        : nome(nome) {}

    string getNome() const { return nome; }

private:
    string nome;
};

class Assento {
public:
    Assento(int numero, bool disponivel)
        : numero(numero), disponivel(disponivel) {}

    int getNumero() const { return numero; }
    bool isDisponivel() const { return disponivel; }

    void reservarAssento() {
        disponivel = false;
    }

private:
    int numero;
    bool disponivel;
};

class Voo {
public:
    Voo(string origem, string destino, int numeroAssentos)
        : origem(origem), destino(destino) {
        for (int i = 1; i <= numeroAssentos; ++i) {
            assentos.push_back(new Assento(i, true));
        }
    }

    string getOrigem() const { return origem; }
    string getDestino() const { return destino; }
    const vector<Assento*>& getAssentos() const { return assentos; }

    bool reservarAssento(int numeroAssento) {
        if (numeroAssento >= 1 && numeroAssento <= assentos.size() && assentos[numeroAssento - 1]->isDisponivel()) {
            assentos[numeroAssento - 1]->reservarAssento();
            return true;
        }
        return false;
    }

private:
    string origem;
    string destino;
    vector<Assento*> assentos;
};

class Passageiro : public Pessoa {
public:
    Passageiro(string nome, Voo& voo, int numeroAssento)
        : Pessoa(nome), voo(voo), numeroAssento(numeroAssento), reservaConfirmada(false) {}

    bool getReservaConfirmada() const { return reservaConfirmada; }
    const Voo& getVoo() const { return voo; }
    int getNumeroAssento() const { return numeroAssento; }

    void confirmarReserva() {
        reservaConfirmada = true;
    }

private:
    Voo& voo;
    int numeroAssento;
    bool reservaConfirmada;
};

vector<Passageiro*> passageiros;
vector<Voo*> voos;

void reservarPassagem(Passageiro* passageiro) {
    passageiros.push_back(passageiro);
}

void adicionarVoo(Voo* voo) {
    voos.push_back(voo);
}

void limparTerminal() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void exibirPassageiros() {
    cout << "Lista de Reservas:" << endl;
    for (const auto& passageiro : passageiros) {
        cout << "Nome: " << passageiro->getNome()
             << ", Destino: " << passageiro->getVoo().getDestino()
             << ", Assento: " << passageiro->getNumeroAssento()
             << ", Reserva Confirmada: " << (passageiro->getReservaConfirmada() ? "Sim" : "Não")
             << endl;
    }
}

void exibirVoosDisponiveis() {
    cout << "Voos Disponíveis:" << endl;
    for (size_t i = 0; i < voos.size(); ++i) {
        cout << i + 1 << ". Origem: " << voos[i]->getOrigem() << ", Destino: " << voos[i]->getDestino() << endl;
    }
}

void exibirAssentosDisponiveis(const Voo& voo) {
    cout << "Assentos Disponíveis para o Voo " << voo.getOrigem() << " - " << voo.getDestino() << ":" << endl;
    for (const auto& assento : voo.getAssentos()) {
        if (assento->isDisponivel()) {
            cout << "Assento " << assento->getNumero() << endl;
        }
    }
}

void exibirReservasRealizadas() {
    cout << "Reservas Realizadas:" << endl;
    for (const auto& passageiro : passageiros) {
        cout << "Nome: " << passageiro->getNome()
             << ", Destino: " << passageiro->getVoo().getDestino()
             << ", Assento: " << passageiro->getNumeroAssento()
             << endl;
    }
}

void exibirReservasRealizadas(string nomePassageiro) {
    bool encontrouReservas = false;

    cout << "Reservas para o passageiro " << nomePassageiro << ":" << endl;
    for (const auto& passageiro : passageiros) {
        if (passageiro->getNome() == nomePassageiro) {
            cout << "Destino: " << passageiro->getVoo().getDestino()
                << ", Assento: " << passageiro->getNumeroAssento()
                << endl;
            encontrouReservas = true;
        }
    }

    if (!encontrouReservas) {
        cout << "Nenhuma reserva encontrada para o passageiro " << nomePassageiro << "." << endl;
    }
}

int main() {
    adicionarVoo(new Voo("Sao Paulo", "Nova York", 10));
    adicionarVoo(new Voo("Paris", "Toquio", 15));
    adicionarVoo(new Voo("Londres", "Sydney", 12));

    int escolha;

    do {
        limparTerminal();
        cout << "Menu:" << endl;
        cout << "1. Fazer Reserva" << endl;
        cout << "2. Conferir Reservas" << endl;
        cout << "0. Sair" << endl;
        cout << "Escolha uma opcao: ";
        cin >> escolha;

        switch (escolha) {
            case 1: {
                exibirVoosDisponiveis();
                int indiceVoo;
                cout << "Escolha o numero do voo para fazer a reserva: ";
                cin >> indiceVoo;

                if (indiceVoo >= 1 && indiceVoo <= voos.size()) {
                    exibirAssentosDisponiveis(*(voos[indiceVoo - 1]));
                    int numeroAssento;
                    cout << "Escolha o numero do assento: ";
                    cin >> numeroAssento;

                    if (voos[indiceVoo - 1]->reservarAssento(numeroAssento)) {
                        string nomePassageiro;
                        cout << "Digite o nome do passageiro: ";
                        cin.ignore();
                        getline(cin, nomePassageiro);

                        Passageiro* novoPassageiro = new Passageiro(nomePassageiro, *(voos[indiceVoo - 1]), numeroAssento);
                        novoPassageiro->confirmarReserva();
                        reservarPassagem(novoPassageiro);

                        cout << "Reserva feita com sucesso!" << endl;
                    } else {
                        cout << "Assento nao disponivel." << endl;
                    }
                } else {
                    cout << "Voo nao valido." << endl;
                }

                break;
            }
            case 2: {
                cout << "1. Exibir Todas as Reservas" << endl;
                cout << "2. Exibir Reservas por Passageiro" << endl;
                cout << "Escolha uma opcao: ";
                int opcaoReservas;
                cin >> opcaoReservas;

                switch (opcaoReservas) {
                    case 1:
                        exibirReservasRealizadas();
                        break;
                    case 2: {
                        string nomePassageiro;
                        cout << "Digite o nome do passageiro: ";
                        cin.ignore();
                        getline(cin, nomePassageiro);

                        exibirReservasRealizadas(nomePassageiro);
                        break;
                    }
                    default:
                        cout << "Opcao invalida. Tente novamente." << endl;
                }

                limparTerminal();
                break;
            }
            case 0:
                cout << "Saindo do programa." << endl;
                break;
            default:
                cout << "Opcao invalida. Tente novamente." << endl;
        }
    } while (escolha != 0);

    for (Passageiro* p : passageiros) {
        delete p;
    }

    for (Voo* v : voos) {
        delete v;
    }

    return 0;
}

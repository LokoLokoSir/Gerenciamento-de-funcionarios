#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>
#include <algorithm>
using namespace std;

struct Funcionario {
    int id;
    string nome;
    string senha;
    bool ativo = true;
};

const int MAX_FUNCIONARIOS = 100;
Funcionario funcionarios[MAX_FUNCIONARIOS];
int totalFuncionarios = 0;

void limparBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string paraMinusculo(string texto) {
    transform(texto.begin(), texto.end(), texto.begin(), ::tolower);
    return texto;
}

int encontrarFuncionarioPorID(int id) {
    for (int i = 0; i < totalFuncionarios; ++i) {
        if (funcionarios[i].id == id && funcionarios[i].ativo) {
            return i;
        }
    }
    return -1;
}

enum TipoUsuario { INVALIDO, ADMINISTRADOR, FUNCIONARIO };

TipoUsuario login() {
    string usuario, senha;
    cout << "==== LOGIN ====\n";
    cout << "Usuario: ";
    cin >> usuario;
    cout << "Senha: ";
    cin >> senha;

    if (usuario == "admin" && senha == "admin123") {
        return ADMINISTRADOR;
    }

    for (int i = 0; i < totalFuncionarios; ++i) {
        if (funcionarios[i].ativo && funcionarios[i].nome == usuario && funcionarios[i].senha == senha) {
            return FUNCIONARIO;
        }
    }

    cout << "Login invalido.\n";
    return INVALIDO;
}

void salvarFuncionarios() {
    ofstream arquivo("funcionarios.txt");
    for (int i = 0; i < totalFuncionarios; ++i) {
        arquivo << funcionarios[i].id << '|'
                << funcionarios[i].nome << '|'
                << funcionarios[i].senha << '|'
                << funcionarios[i].ativo << '\n';
    }
    arquivo.close();
}

void carregarFuncionarios() {
    ifstream arquivo("funcionarios.txt");
    if (!arquivo.is_open()) return;

    string linha;
    while (getline(arquivo, linha)) {
        stringstream ss(linha);
        string parte;
        Funcionario f;

        getline(ss, parte, '|');
        f.id = stoi(parte);

        getline(ss, parte, '|');
        f.nome = parte;

        getline(ss, parte, '|');
        f.senha = parte;

        getline(ss, parte, '|');
        f.ativo = (parte == "1");

        funcionarios[totalFuncionarios++] = f;
    }

    arquivo.close();
}

void cadastrarFuncionario() {
    if (totalFuncionarios >= MAX_FUNCIONARIOS) {
        cout << "Limite de funcionarios atingido!\n";
        return;
    }

    cin.ignore();
    cout << "Nome do funcionario (login): ";
    getline(cin, funcionarios[totalFuncionarios].nome);

    cout << "Senha do funcionario: ";
    getline(cin, funcionarios[totalFuncionarios].senha);

    funcionarios[totalFuncionarios].id = totalFuncionarios + 1;
    funcionarios[totalFuncionarios].ativo = true;

    cout << "Funcionario cadastrado com ID: " << funcionarios[totalFuncionarios].id << "\n";
    totalFuncionarios++;
    salvarFuncionarios();
}

void listarFuncionarios() {
    cout << "\n=== LISTA DE FUNCIONARIOS ===\n";
    for (int i = 0; i < totalFuncionarios; ++i) {
        if (funcionarios[i].ativo) {
            cout << "ID: " << funcionarios[i].id << "\n";
            cout << "Nome (Login): " << funcionarios[i].nome << "\n";
            cout << "--------------------------\n";
        }
    }
}

void editarFuncionario() {
    int id;
    cout << "ID do funcionario a editar: ";
    cin >> id;

    int index = encontrarFuncionarioPorID(id);
    if (index == -1) {
        cout << "Funcionario nao encontrado!\n";
        return;
    }

    cin.ignore();
    cout << "Novo nome (login): ";
    getline(cin, funcionarios[index].nome);

    cout << "Nova senha: ";
    getline(cin, funcionarios[index].senha);

    cout << "Funcionario atualizado!\n";
    salvarFuncionarios();
}

void removerFuncionario() {
    int id;
    cout << "ID do funcionario a remover: ";
    cin >> id;

    int index = encontrarFuncionarioPorID(id);
    if (index == -1) {
        cout << "Funcionario nao encontrado!\n";
        return;
    }

    funcionarios[index].ativo = false;
    funcionarios[index].senha = "";
    cout << "Funcionario removido!\n";
    salvarFuncionarios();
}

void buscarFuncionario() {
    cout << "\nBuscar por:\n1. ID\n2. Nome\nEscolha: ";
    int tipoBusca;
    cin >> tipoBusca;

    if (tipoBusca == 1) {
        int id;
        cout << "Digite o ID: ";
        cin >> id;
        int index = encontrarFuncionarioPorID(id);
        if (index != -1) {
            cout << "\nID: " << funcionarios[index].id << "\n";
            cout << "Nome: " << funcionarios[index].nome << "\n";
        } else {
            cout << "Funcionario nao encontrado.\n";
        }
    } else if (tipoBusca == 2) {
        cin.ignore();
        string termo;
        cout << "Digite parte do nome: ";
        getline(cin, termo);
        termo = paraMinusculo(termo);

        bool encontrou = false;
        for (int i = 0; i < totalFuncionarios; ++i) {
            if (funcionarios[i].ativo && paraMinusculo(funcionarios[i].nome).find(termo) != string::npos) {
                cout << "\nID: " << funcionarios[i].id << "\n";
                cout << "Nome: " << funcionarios[i].nome << "\n";
                cout << "--------------------------\n";
                encontrou = true;
            }
        }

        if (!encontrou) {
            cout << "Nenhum funcionario encontrado.\n";
        }
    } else {
        cout << "Opcao invalida!\n";
    }
}

int main() {
    TipoUsuario tipoUsuario = login();
    if (tipoUsuario == INVALIDO) return 0;

    carregarFuncionarios();

    while (true) {
        cout << "\n====== MENU DE OPCOES ======\n";
        cout << "1. Listar Funcionarios\n";
        cout << "2. Buscar Funcionario\n";

        if (tipoUsuario == ADMINISTRADOR) {
            cout << "3. Cadastrar Funcionario\n";
            cout << "4. Editar Funcionario\n";
            cout << "5. Remover Funcionario\n";
            cout << "6. Sair\n";
        } else {
            cout << "3. Sair\n";
        }

        cout << "Escolha: ";
        int opcao;
        cin >> opcao;

        if (cin.fail()) {
            limparBuffer();
            cout << "Entrada invalida!\n";
            continue;
        }

        if (tipoUsuario == ADMINISTRADOR) {
            switch (opcao) {
                case 1: listarFuncionarios(); break;
                case 2: buscarFuncionario(); break;
                case 3: cadastrarFuncionario(); break;
                case 4: editarFuncionario(); break;
                case 5: removerFuncionario(); break;
                case 6: cout << "Saindo...\n"; return 0;
                default: cout << "Opcao invalida!\n"; break;
            }
        } else if (tipoUsuario == FUNCIONARIO) {
            switch (opcao) {
                case 1: listarFuncionarios(); break;
                case 2: buscarFuncionario(); break;
                case 3: cout << "Saindo...\n"; return 0;
                default: cout << "Opcao invalida!\n"; break;
            }
        }
    }

    return 0;
}
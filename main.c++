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
    string tarefa = "null";
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

void salvarFuncionarios() {
    ofstream arquivo("funcionarios.txt");
    for (int i = 0; i < totalFuncionarios; ++i) {
        arquivo << funcionarios[i].id << '|'
                << funcionarios[i].nome << '|'
                << funcionarios[i].senha << '|'
                << funcionarios[i].tarefa << '|'
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
        f.tarefa = parte;

        getline(ss, parte, '|');
        f.ativo = (parte == "1");

        funcionarios[totalFuncionarios++] = f;
    }

    arquivo.close();
}

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

    cout << "Login invalido." << endl;
    return INVALIDO;
}

void cadastrarFuncionario() {
    char opcao;
    cout << "Deseja cadastrar um novo funcionario? (s/n): ";
    cin >> opcao;
    if (opcao != 's' && opcao != 'S') return;

    if (totalFuncionarios >= MAX_FUNCIONARIOS) {
        cout << "Limite de funcionarios atingido!" << endl;
        return;
    }

    cin.ignore();
    cout << "Nome do funcionario (login): ";
    getline(cin, funcionarios[totalFuncionarios].nome);

    cout << "Senha do funcionario: ";
    getline(cin, funcionarios[totalFuncionarios].senha);

    funcionarios[totalFuncionarios].id = totalFuncionarios + 1;
    funcionarios[totalFuncionarios].ativo = true;

    cout << "Funcionario cadastrado com ID: " << funcionarios[totalFuncionarios].id << endl;
    totalFuncionarios++;
    salvarFuncionarios();
}

void listarFuncionarios() {
    char voltar;
    cout << "\n=== LISTA DE FUNCIONARIOS ===" << endl;
    for (int i = 0; i < totalFuncionarios; ++i) {
        if (funcionarios[i].ativo) {
            cout << "ID: " << funcionarios[i].id << endl;
            cout << "Nome (Login): " << funcionarios[i].nome << endl;
            cout << "--------------------------" << endl;
        }
    }
    cout << "Digite qualquer tecla para voltar ao menu...";
    cin.ignore();
    cin.get();
}

void editarFuncionario() {
    int id;
    cout << "ID do funcionario a editar (0 para voltar): ";
    cin >> id;
    if (id == 0) return;

    int index = encontrarFuncionarioPorID(id);
    if (index == -1) {
        cout << "Funcionario nao encontrado!" << endl;
        return;
    }

    cin.ignore();
    cout << "Novo nome (login): ";
    getline(cin, funcionarios[index].nome);

    cout << "Nova senha: ";
    getline(cin, funcionarios[index].senha);

    cout << "Funcionario atualizado!" << endl;
    salvarFuncionarios();
}

void removerFuncionario() {
    int id;
    cout << "ID do funcionario a remover (0 para voltar): ";
    cin >> id;
    if (id == 0) return;

    int index = encontrarFuncionarioPorID(id);
    if (index == -1) {
        cout << "Funcionario nao encontrado!" << endl;
        return;
    }

    funcionarios[index].ativo = false;
    funcionarios[index].senha = "";
    cout << "Funcionario removido!" << endl;
    salvarFuncionarios();
}

void buscarFuncionario() {
    while (true) {
        cout << "\nBuscar por:\n1. ID\n2. Nome\n3. Voltar\n4. Voltar ao Login\nEscolha: ";
        int tipoBusca;
        cin >> tipoBusca;

        if (tipoBusca == 1) {
            int id;
            cout << "Digite o ID: ";
            cin >> id;
            if (id == 0) continue;
            int index = encontrarFuncionarioPorID(id);
            if (index != -1) {
                cout << "\nID: " << funcionarios[index].id << endl;
                cout << "Nome: " << funcionarios[index].nome << endl;
            } else {
                cout << "Funcionario nao encontrado." << endl;
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
                    cout << "\nID: " << funcionarios[i].id << endl;
                    cout << "Nome: " << funcionarios[i].nome << endl;
                    cout << "--------------------------" << endl;
                    encontrou = true;
                }
            }

            if (!encontrou) {
                cout << "Nenhum funcionario encontrado." << endl;
            }
        } else if (tipoBusca == 3) {
            return;
        } else if (tipoBusca == 4) {
            return; // volta ao login
        } else {
            cout << "Opcao invalida!" << endl;
        }
    }
}

void tarefas()
{
    int index;
    cout << "Digite o nome do funcionario: ";
    string busca;
    cin >> busca;

    bool encontrou = false;
    for (int i = 0; i < totalFuncionarios; ++i) {
        if (funcionarios[i].ativo && paraMinusculo(funcionarios[i].nome).find(busca) != string::npos) {
            index = i;
            encontrou = true;
        }
    }

    if (!encontrou) {
        cout << "Nenhum funcionario encontrado.\n";
    }else 
    {
        cout << "Digite a nova tarefa do funcionario: ";
        cin.ignore();
        string newTarefa;
        getline(cin, funcionarios[index].tarefa);

        salvarFuncionarios();

        cout << funcionarios[index].tarefa;
    }
}

int main() {
    while (true) {
        carregarFuncionarios();
        TipoUsuario tipoUsuario = login();
        if (tipoUsuario == INVALIDO) continue;

        bool continuar = true;
        while (continuar) {
            cout << "\n====== MENU DE OPCOES ======" << endl;
            cout << "1. Listar Funcionarios" << endl;
            cout << "2. Buscar Funcionario" << endl;

            if (tipoUsuario == ADMINISTRADOR) {
                cout << "3. Cadastrar Funcionario" << endl;
                cout << "4. Editar Funcionario" << endl;
                cout << "5. Remover Funcionario" << endl;
                cout << "6. tarefas" << endl;
                cout << "7. Voltar ao Login" << endl;
                cout << "8. Sair" << endl;
            } else {
                cout << "3. Voltar ao Login" << endl;
                cout << "4. Sair" << endl;
            }

            cout << "Escolha: ";
            int opcao;
            cin >> opcao;

            if (cin.fail()) {
                limparBuffer();
                cout << "Entrada invalida!" << endl;
                continue;
            }

            if (tipoUsuario == ADMINISTRADOR) {
                switch (opcao) {
                    case 1: listarFuncionarios(); break;
                    case 2: buscarFuncionario(); break;
                    case 3: cadastrarFuncionario(); break;
                    case 4: editarFuncionario(); break;
                    case 5: removerFuncionario(); break;
                    case 6: tarefas(); break;
                    case 7: continuar = false; break;
                    case 8: cout << "Saindo..." << endl; return 0;
                    default: cout << "Opcao invalida!" << endl;
                }
            } else {
                switch (opcao) {
                    case 1: listarFuncionarios(); break;
                    case 2: buscarFuncionario(); break;
                    case 3: continuar = false; break;
                    case 4: cout << "Saindo..." << endl; return 0;
                    default: cout << "Opcao invalida!" << endl;
                }
            }
        }
    }
    return 0;
}

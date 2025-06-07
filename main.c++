#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>
#include <algorithm>
#include <iomanip>
using namespace std;

// Estrutura para funcionários
struct Funcionario {
    int id;
    string nome;
    string senha;
    string cpf;
    string cargo;
    double salario;
    bool ativo = true;
};

// Estrutura para tarefas
struct Tarefa {
    int id_tarefa;
    int id_funcionario;
    string descricao;
    string data_termino;
    bool concluida = false;
};

// Constantes para limites das matrizes
const int MAX_FUNCIONARIOS = 100;
const int MAX_TAREFAS = 200;

// Matrizes globais
Funcionario funcionarios[MAX_FUNCIONARIOS];
Tarefa tarefas[MAX_TAREFAS];
int totalFuncionarios = 0;
int totalTarefas = 0;

enum TipoUsuario { INVALIDO, ADMINISTRADOR, FUNCIONARIO };

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

// Função para salvar funcionários em arquivo
void salvarFuncionarios() {
    ofstream arquivo("funcionarios.txt");
    for (int i = 0; i < totalFuncionarios; ++i) {
        arquivo << funcionarios[i].id << '|'
                << funcionarios[i].nome << '|'
                << funcionarios[i].senha << '|'
                << funcionarios[i].cpf << '|'
                << funcionarios[i].cargo << '|'
                << funcionarios[i].ativo << '\n';
    }
    arquivo.close();
}

// Função para salvar tarefas em arquivo
void salvarTarefas() {
    ofstream arquivo("tarefas.txt");
    for (int i = 0; i < totalTarefas; ++i) {
        arquivo << tarefas[i].id_tarefa << '|'
                << tarefas[i].id_funcionario << '|'
                << tarefas[i].descricao << '|'
                << tarefas[i].data_termino << '|'
                << tarefas[i].concluida << '\n';
    }
    arquivo.close();
}

// Função para carregar funcionários do arquivo
void carregarFuncionarios() {
    ifstream arquivo("funcionarios.txt");
    if (!arquivo.is_open()) return;

    string linha;
    while (getline(arquivo, linha) && totalFuncionarios < MAX_FUNCIONARIOS) {
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
        f.cpf = parte;

        getline(ss, parte, '|');
        f.cargo = parte;

        getline(ss, parte, '|');
        f.ativo = (parte == "1");

        funcionarios[totalFuncionarios++] = f;
    }
    arquivo.close();
}

// Função para carregar tarefas do arquivo
void carregarTarefas() {
    ifstream arquivo("tarefas.txt");
    if (!arquivo.is_open()) return;

    string linha;
    while (getline(arquivo, linha) && totalTarefas < MAX_TAREFAS) {
        stringstream ss(linha);
        string parte;
        Tarefa t;

        getline(ss, parte, '|');
        t.id_tarefa = stoi(parte);

        getline(ss, parte, '|');
        t.id_funcionario = stoi(parte);

        getline(ss, parte, '|');
        t.descricao = parte;

        getline(ss, parte, '|');
        t.data_termino = parte;

        getline(ss, parte, '|');
        t.concluida = (parte == "1");

        tarefas[totalTarefas++] = t;
    }
    arquivo.close();
}

TipoUsuario login() {
    string usuario, senha;
    cout << "\n==== LOGIN ====\n";
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

    limparBuffer();
    cout << "Nome do funcionario (login): ";
    getline(cin, funcionarios[totalFuncionarios].nome);

    cout << "Senha do funcionario: ";
    getline(cin, funcionarios[totalFuncionarios].senha);

    cout << "CPF do funcionario: ";
    getline(cin, funcionarios[totalFuncionarios].cpf);

    cout << "Cargo do funcionario: ";
    getline(cin, funcionarios[totalFuncionarios].cargo);

    funcionarios[totalFuncionarios].id = totalFuncionarios + 1;
    funcionarios[totalFuncionarios].ativo = true;

    cout << "Funcionario cadastrado com ID: " << funcionarios[totalFuncionarios].id << endl;
    totalFuncionarios++;
    salvarFuncionarios();
}

void listarFuncionarios() {
    cout << "\n=== LISTA DE FUNCIONARIOS ===" << endl;
    cout << left << setw(5) << "ID" << setw(20) << "NOME" << setw(15) << "CPF" << setw(20) << "CARGO" << endl;
    cout << string(60, '-') << endl;
    
    for (int i = 0; i < totalFuncionarios; ++i) {
        if (funcionarios[i].ativo) {
            cout << left << setw(5) << funcionarios[i].id 
                 << setw(20) << funcionarios[i].nome 
                 << setw(15) << funcionarios[i].cpf 
                 << setw(20) << funcionarios[i].cargo << endl;
        }
    }
    cout << "\nPressione Enter para continuar...";
    limparBuffer();
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

    limparBuffer();
    cout << "Novo nome (login): ";
    getline(cin, funcionarios[index].nome);

    cout << "Nova senha: ";
    getline(cin, funcionarios[index].senha);

    cout << "Novo CPF: ";
    getline(cin, funcionarios[index].cpf);

    cout << "Novo cargo: ";
    getline(cin, funcionarios[index].cargo);

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
        cout << "\nBuscar por:\n1. ID\n2. Nome\n3. Voltar\nEscolha: ";
        int tipoBusca;
        cin >> tipoBusca;

        if (tipoBusca == 1) {
            int id;
            cout << "Digite o ID: ";
            cin >> id;
            if (id == 0) continue;
            int index = encontrarFuncionarioPorID(id);
            if (index != -1) {
                cout << "\n=== FUNCIONARIO ENCONTRADO ===" << endl;
                cout << "ID: " << funcionarios[index].id << endl;
                cout << "Nome: " << funcionarios[index].nome << endl;
                cout << "CPF: " << funcionarios[index].cpf << endl;
                cout << "Cargo: " << funcionarios[index].cargo << endl;
            } else {
                cout << "Funcionario nao encontrado." << endl;
            }
        } else if (tipoBusca == 2) {
            limparBuffer();
            string termo;
            cout << "Digite parte do nome: ";
            getline(cin, termo);
            termo = paraMinusculo(termo);

            bool encontrou = false;
            for (int i = 0; i < totalFuncionarios; ++i) {
                if (funcionarios[i].ativo && paraMinusculo(funcionarios[i].nome).find(termo) != string::npos) {
                    cout << "\nID: " << funcionarios[i].id << endl;
                    cout << "Nome: " << funcionarios[i].nome << endl;
                    cout << "CPF: " << funcionarios[i].cpf << endl;
                    cout << "Cargo: " << funcionarios[i].cargo << endl;
                    cout << "--------------------------" << endl;
                    encontrou = true;
                }
            }

            if (!encontrou) {
                cout << "Nenhum funcionario encontrado." << endl;
            }
        } else if (tipoBusca == 3) {
            return;
        } else {
            cout << "Opcao invalida!" << endl;
        }
    }
}

void adicionarTarefa() {
    if (totalTarefas >= MAX_TAREFAS) {
        cout << "Limite de tarefas atingido!" << endl;
        return;
    }

    int id_funcionario;
    cout << "ID do funcionario para atribuir tarefa: ";
    cin >> id_funcionario;

    int index = encontrarFuncionarioPorID(id_funcionario);
    if (index == -1) {
        cout << "Funcionario nao encontrado!" << endl;
        return;
    }

    limparBuffer();
    cout << "Descricao da tarefa: ";
    getline(cin, tarefas[totalTarefas].descricao);

    cout << "Data de termino (DD/MM/AAAA): ";
    getline(cin, tarefas[totalTarefas].data_termino);

    tarefas[totalTarefas].id_tarefa = totalTarefas + 1;
    tarefas[totalTarefas].id_funcionario = id_funcionario;
    tarefas[totalTarefas].concluida = false;

    cout << "Tarefa atribuida com sucesso!" << endl;
    cout << "Funcionario: " << funcionarios[index].nome << endl;
    cout << "Tarefa: " << tarefas[totalTarefas].descricao << endl;
    
    totalTarefas++;
    salvarTarefas();
}

void listarTarefas() {
    cout << "\n=== LISTA DE TODAS AS TAREFAS ===" << endl;
    cout << left << setw(8) << "ID_TAR" << setw(8) << "ID_FUNC" << setw(20) << "FUNCIONARIO" 
         << setw(30) << "DESCRICAO" << setw(12) << "DATA_TERM" << setw(10) << "STATUS" << endl;
    cout << string(88, '-') << endl;
    
    for (int i = 0; i < totalTarefas; ++i) {
        int index_func = encontrarFuncionarioPorID(tarefas[i].id_funcionario);
        string nome_func = (index_func != -1) ? funcionarios[index_func].nome : "REMOVIDO";
        string status = tarefas[i].concluida ? "CONCLUIDA" : "PENDENTE";
        
        cout << left << setw(8) << tarefas[i].id_tarefa
             << setw(8) << tarefas[i].id_funcionario
             << setw(20) << nome_func
             << setw(30) << tarefas[i].descricao
             << setw(12) << tarefas[i].data_termino
             << setw(10) << status << endl;
    }
    cout << "\nPressione Enter para continuar...";
    limparBuffer();
    cin.get();
}

void verificarTarefasFuncionario(const string& nomeUsuario) {
    int id_funcionario = -1;
    
    // Encontrar ID do funcionário logado
    for (int i = 0; i < totalFuncionarios; ++i) {
        if (funcionarios[i].ativo && funcionarios[i].nome == nomeUsuario) {
            id_funcionario = funcionarios[i].id;
            break;
        }
    }

    if (id_funcionario == -1) {
        cout << "Funcionario nao encontrado!" << endl;
        return;
    }

    cout << "\n=== SUAS TAREFAS ===" << endl;
    cout << left << setw(8) << "ID_TAR" << setw(35) << "DESCRICAO" << setw(12) << "DATA_TERM" << setw(10) << "STATUS" << endl;
    cout << string(65, '-') << endl;
    
    bool temTarefas = false;
    for (int i = 0; i < totalTarefas; ++i) {
        if (tarefas[i].id_funcionario == id_funcionario) {
            string status = tarefas[i].concluida ? "CONCLUIDA" : "PENDENTE";
            cout << left << setw(8) << tarefas[i].id_tarefa
                 << setw(35) << tarefas[i].descricao
                 << setw(12) << tarefas[i].data_termino
                 << setw(10) << status << endl;
            temTarefas = true;
        }
    }
    
    if (!temTarefas) {
        cout << "Nenhuma tarefa atribuida." << endl;
    }
    
    cout << "\nPressione Enter para continuar...";
    limparBuffer();
    cin.get();
}

void marcarTarefaConcluida(const string& nomeUsuario) {
    int id_funcionario = -1;
    
    // Encontrar ID do funcionário logado
    for (int i = 0; i < totalFuncionarios; ++i) {
        if (funcionarios[i].ativo && funcionarios[i].nome == nomeUsuario) {
            id_funcionario = funcionarios[i].id;
            break;
        }
    }

    if (id_funcionario == -1) {
        cout << "Funcionario nao encontrado!" << endl;
        return;
    }

    int id_tarefa;
    cout << "Digite o ID da tarefa a marcar como concluida: ";
    cin >> id_tarefa;

    bool encontrou = false;
    for (int i = 0; i < totalTarefas; ++i) {
        if (tarefas[i].id_tarefa == id_tarefa && tarefas[i].id_funcionario == id_funcionario) {
            tarefas[i].concluida = true;
            cout << "Tarefa marcada como concluida!" << endl;
            salvarTarefas();
            encontrou = true;
            break;
        }
    }

    if (!encontrou) {
        cout << "Tarefa nao encontrada ou nao pertence a voce!" << endl;
    }
}

void verFuncionarios() {
    cout << "\n=== LISTA DE FUNCIONARIOS ===" << endl;
    cout << left << setw(5) << "ID" << setw(20) << "NOME" << setw(15) << "CPF" << setw(20) << "CARGO" << endl;
    cout << string(60, '-') << endl;

    for (int i = 0; i < totalFuncionarios; ++i) {
        if (funcionarios[i].ativo) {
            cout << left << setw(5) << funcionarios[i].id 
                 << setw(20) << funcionarios[i].nome 
                 << setw(15) << funcionarios[i].cpf 
                 << setw(20) << funcionarios[i].cargo << endl;
        }
    }
    cout << "\nPressione Enter para continuar...";
    limparBuffer();
    cin.get();
}

int main() {
    carregarFuncionarios();
    carregarTarefas();

    while (true) {
        TipoUsuario tipoUsuario = login();
        if (tipoUsuario == INVALIDO) continue;

        string usuarioLogado;

        if (tipoUsuario == FUNCIONARIO) {
            cout << "Digite novamente seu nome de usuario para confirmar: ";
            cin >> usuarioLogado;

            bool continuarFuncionario = true;
            while (continuarFuncionario) {
                cout << "\n===== MENU FUNCIONARIO =====\n";
                cout << "1. Ver minhas tarefas\n";
                cout << "2. Marcar tarefa como concluida\n";
                cout << "3. Ver funcionarios\n";
                cout << "4. Buscar funcionario\n";
                cout << "5. Voltar ao login\n";
                cout << "6. Sair\n";
                cout << "Escolha: ";
                int opcao;
                cin >> opcao;

                if (cin.fail()) {
                    limparBuffer();
                    cout << "Entrada invalida!\n";
                    continue;
                }

                switch (opcao) {
                    case 1: verificarTarefasFuncionario(usuarioLogado); break;
                    case 2: marcarTarefaConcluida(usuarioLogado); break;
                    case 3: verFuncionarios(); break;
                    case 4: buscarFuncionario(); break;
                    case 5: continuarFuncionario = false; break;
                    case 6: return 0;
                    default: cout << "Opcao invalida!\n";
                }
            }

        } else if (tipoUsuario == ADMINISTRADOR) {
            bool continuarAdmin = true;
            while (continuarAdmin) {
                cout << "\n====== MENU ADMINISTRADOR ======\n";
                cout << "1. Listar Funcionarios\n";
                cout << "2. Buscar Funcionario\n";
                cout << "3. Cadastrar Funcionario\n";
                cout << "4. Editar Funcionario\n";
                cout << "5. Remover Funcionario\n";
                cout << "6. Adicionar Tarefa\n";
                cout << "7. Listar Todas as Tarefas\n";
                cout << "8. Voltar ao Login\n";
                cout << "9. Sair\n";
                cout << "Escolha: ";
                int opcao;
                cin >> opcao;

                if (cin.fail()) {
                    limparBuffer();
                    cout << "Entrada invalida!\n";
                    continue;
                }

                switch (opcao) {
                    case 1: listarFuncionarios(); break;
                    case 2: buscarFuncionario(); break;
                    case 3: cadastrarFuncionario(); break;
                    case 4: editarFuncionario(); break;
                    case 5: removerFuncionario(); break;
                    case 6: adicionarTarefa(); break;
                    case 7: listarTarefas(); break;
                    case 8: continuarAdmin = false; break;
                    case 9: return 0;
                    default: cout << "Opcao invalida!\n";
                }
            }
        }
    }
    return 0;
}
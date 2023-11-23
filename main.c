#include <stdio.h>
#include <locale.h>

void carregarBancoDados()
{
    printf("Opção 1 - Carregar Banco de Dados\n");
    // Adicionar lógica
}

void cadastrarProduto()
{
    printf("Opção 2 - Cadastrar produto\n");
    // Adicionar lógica
}

void alterarProduto()
{
    printf("Opção 3 - Alterar produto\n");
    // Adicionar lógica
}

void excluirProduto()
{
    printf("Opção 4 - Excluir produto\n");
    // Adicionar lógica
}

void listarProdutos()
{
    printf("Opção 5 - Listar produtos\n");
    // Adicionar lógica
}

void listarMenu()
{
    printf("\n=== Menu de Navegação ===\n");
    printf("1 - Carregar Banco de Dados\n");
    printf("2 - Cadastrar produto\n");
    printf("3 - Alterar produto\n");
    printf("4 - Excluir produto\n");
    printf("5 - Listar produtos\n");
    printf("0 - Sair\n");
}

int main()
{
    setlocale(LC_ALL, "");
    int opcao;

    do
    {
        // Exibindo o menu
        listarMenu();

        // Solicitando a escolha do usuário
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        // Executando a opção escolhida
        switch (opcao)
        {
        case 1:
            carregarBancoDados();
            break;
        case 2:
            cadastrarProduto();
            break;
        case 3:
            alterarProduto();
            break;
        case 4:
            excluirProduto();
            break;
        case 5:
            listarProdutos();
            break;
        case 0:
            printf("Saindo do programa. Até mais!\n");
            break;
        default:
            printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
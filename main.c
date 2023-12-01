#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

typedef struct
{
	int id;
	char nome[51];
    int quantidade;
    float valor;
}  Produto;

char* removeEspacos(const char *frase)
{
    int tamanho = strlen(frase);

    if (tamanho > 0 && frase[tamanho - 1] == '\n') {
        tamanho--;
    }
    
    char *fraseSemEspacos = (char*)malloc((tamanho + 1) * sizeof(char));

    strncpy(fraseSemEspacos, frase, tamanho);

    fraseSemEspacos[tamanho] = '\0';

    return fraseSemEspacos;
}

FILE* abrirArquivo(const char* nomeArquivo, const char* modo) {
    FILE* arquivo = fopen(nomeArquivo, modo);

    if (arquivo == NULL && modo!="r") {
        fprintf(stderr, "Erro ao abrir o arquivo %s.\n", nomeArquivo);
    }

    return arquivo;
}

void fecharArquivo(FILE* arquivo)
{
    if (arquivo != NULL) {
        fclose(arquivo);
    }
}

Produto *carregarBancoDados(int *tamanho)
{
    FILE* arquivo = abrirArquivo("dados.txt", "r");
    *tamanho = 0;
    Produto *vetorProdutos = NULL;

    if(arquivo == NULL) {
        return vetorProdutos;
    }

    int id;
    char nome[50];
    int quantidade;
    float valor;

    while (fscanf(arquivo, "%d,%49[^,],%d,%f;\n", &id, nome, &quantidade, &valor) == 4)
    {
        (*tamanho)++;
        vetorProdutos = (Produto*)realloc(vetorProdutos, (*tamanho) * sizeof(Produto));

        if (vetorProdutos == NULL)
        {
            printf("Erro na realoca��o de mem�ria.\n");
            exit(1);
        } else {
            vetorProdutos[*tamanho - 1].id = id;
            strcpy(vetorProdutos[*tamanho - 1].nome, nome);
            vetorProdutos[*tamanho - 1].quantidade = quantidade;
            vetorProdutos[*tamanho - 1].valor = valor;
        }
        }

    fecharArquivo(arquivo);
    return vetorProdutos;
}

void cadastrarProduto(int ultimoId)
{
    FILE *arquivo = abrirArquivo("dados.txt", "a");

    int quantidade;
    printf("Quantos produtos voc� quer cadastrar?\n");
    scanf("%d", &quantidade);
    if (quantidade <= 0) {
    printf("Quantidade inv�lida. Tente novamente.\n");
    return;
    }

	int i;
    for(i = 0; i < quantidade; i++)
    {
        getchar();
        Produto produto;
        printf("Digite o nome do produto: \n");
        fflush(stdin);
        fgets(produto.nome, 50, stdin);

        printf("Digite a quantidade do produto no estoque: \n");
        scanf("%d", &produto.quantidade);

        printf("Digite o valor de venda do produto: \n");
        scanf("%f", &produto.valor);

        Produto dados[1];
        dados[0].id = ultimoId+1;
        strcpy(dados[0].nome, removeEspacos(produto.nome));
        dados[0].quantidade = produto.quantidade;
        dados[0].valor = produto.valor;

        fprintf(arquivo, "%d,%s,%d,%f;\n", dados[0].id, dados[0].nome, dados[0].quantidade, dados[0].valor);
        ultimoId++;

    }
    fecharArquivo(arquivo);
}



void excluirProduto()
{
    printf("Op��o 4 - Excluir produto\n");
    // Adicionar l�gica
}

void listarProdutos(Produto *vetorProdutos, int tamanho)
{
	//verificar caso de exclusao de produtos no meio da listagem
	int i;
    for (i = 0; i < tamanho; i++)
    {
        printf("ID: %d, Nome: %s, Quantidade: %d, Valor: %.2f\n", vetorProdutos[i].id, vetorProdutos[i].nome, vetorProdutos[i].quantidade, vetorProdutos[i].valor);
    }
}

// void listaProdutoId(int id) {
//     int i;
//     printf("ID: %d, Nome: %s, Quantidade: %d, Valor: %.2f\n", vetorProdutos)
// }

void listarMenu()
{
    printf("\n=== Menu de Navega��o ===\n");
    printf("1 - Carregar Banco de Dados\n");
    printf("2 - Cadastrar produto\n");
    printf("3 - Alterar produto\n");
    printf("4 - Excluir produto\n");
    printf("5 - Listar produtos\n");
    printf("0 - Sair\n");
}

void alterarProduto()
{
    printf("Opção 3 - Alterar produto\n");

    int ultimoId;

    printf("======= LISTAGEM DE PRODUTOS =======\n");

    Produto *vetorProdutosListagem = carregarBancoDados(&ultimoId);
    listarProdutos(vetorProdutosListagem, ultimoId);

    printf("=====================================\n");

    printf("Digite o id do produto que gostaria de alterar:\n");

    int productId;
    scanf("%d", &productId);

     // Procura o produto pelo index fornecido
    int index = -1;
    int i;
    for ( i = 0; i < ultimoId; i++)
    {
        if (vetorProdutosListagem[i].id == productId)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printf("Produto com ID %d não encontrado.\n", productId);
        return;
    }

    printf("Produto selecionado:\n");
    printf("ID: %d, Nome: %s, Quantidade: %d, Valor: %.2f\n", vetorProdutosListagem[index].id, vetorProdutosListagem[index].nome, vetorProdutosListagem[index].quantidade, vetorProdutosListagem[index].valor);

    printf("Informe qual o atributo que gostaria de alterar:\n");
    printf("1 - Nome\n");
    printf("2 - Quantidade\n");
    printf("3 - Valor\n");

    



    // FILE *arquivo = abrirArquivo("dados.txt", "a");
    

    // Adicionar l�gica
}

int main()
{
    int ultimoId;
    setlocale(LC_ALL, "");
    int opcao;

    do
    {
        // Exibindo o menu
        listarMenu();

        // Solicitando a escolha do usu�rio
        printf("Escolha uma op��o: ");
        scanf("%d", &opcao);

        // Executando a op��o escolhida
        switch (opcao)
        {
         case 1:
            //  carregarBancoDados();
            //  break;
        case 2: {
             Produto *vetorProdutos = carregarBancoDados(&ultimoId);
            cadastrarProduto(ultimoId);
            free(vetorProdutos);
            break;
        } 
        case 3:
            alterarProduto();
            break;
        case 4:
            excluirProduto();
            break;
        case 5: {
            Produto *vetorProdutosListagem = carregarBancoDados(&ultimoId);
            listarProdutos(vetorProdutosListagem, ultimoId);
            free(vetorProdutosListagem);
            break;
        }
        case 0:
            printf("Saindo do programa. At� mais!\n");
            break;
        default:
            printf("Op��o inv�lida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

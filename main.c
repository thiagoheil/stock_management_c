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
} Produto;

char *removeEspacos(const char *frase)
{
    int tamanho = strlen(frase);

    if (tamanho > 0 && frase[tamanho - 1] == '\n')
    {
        tamanho--;
    }

    char *fraseSemEspacos = (char *)malloc((tamanho + 1) * sizeof(char));

    strncpy(fraseSemEspacos, frase, tamanho);

    fraseSemEspacos[tamanho] = '\0';

    return fraseSemEspacos;
}

FILE *abrirArquivo(const char *nomeArquivo, const char *modo)
{
    FILE *arquivo = fopen(nomeArquivo, modo);

    if (arquivo == NULL && modo != "r")
    {
        fprintf(stderr, "Erro ao abrir o arquivo %s.\n", nomeArquivo);
    }

    return arquivo;
}

void fecharArquivo(FILE *arquivo)
{
    if (arquivo != NULL)
    {
        fclose(arquivo);
    }
}

Produto *carregarBancoDados(int *quantidadeProdutos, int *ultimoId)
{
    FILE *arquivo = abrirArquivo("dados.txt", "r");
    *quantidadeProdutos = 0;
    *ultimoId = 0;
    Produto *vetorProdutos = NULL;

    if (arquivo == NULL)
    {
        return vetorProdutos;
    }

    int id;
    char nome[50];
    int quantidade;
    float valor;

    while (fscanf(arquivo, "%d,%49[^,],%d,%f;\n", &id, nome, &quantidade, &valor) == 4)
    {
        (*quantidadeProdutos)++;
        (*ultimoId) = id;
        vetorProdutos = (Produto *)realloc(vetorProdutos, (*quantidadeProdutos) * sizeof(Produto));

        if (vetorProdutos == NULL)
        {
            printf("Erro na realocacao de memoria.\n");
            exit(1);
        }
        else
        {
            vetorProdutos[*quantidadeProdutos - 1].id = id;
            strcpy(vetorProdutos[*quantidadeProdutos - 1].nome, nome);
            vetorProdutos[*quantidadeProdutos - 1].quantidade = quantidade;
            vetorProdutos[*quantidadeProdutos - 1].valor = valor;
        }
    }

    fecharArquivo(arquivo);
    return vetorProdutos;
}

void cadastrarProduto(int ultimoId)
{
    FILE *arquivo = abrirArquivo("dados.txt", "a");

    int quantidade;
    printf("Quantos produtos voce quer cadastrar?\n");
    scanf("%d", &quantidade);
    if (quantidade <= 0)
    {
        printf("Quantidade invalida. Tente novamente.\n");
        return;
    }

    int i;
    for (i = 0; i < quantidade; i++)
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
        dados[0].id = ultimoId + 1;
        strcpy(dados[0].nome, removeEspacos(produto.nome));
        dados[0].quantidade = produto.quantidade;
        dados[0].valor = produto.valor;

        fprintf(arquivo, "%d,%s,%d,%f;\n", dados[0].id, dados[0].nome, dados[0].quantidade, dados[0].valor);
        printf("Produto cadastrado com sucesso!\n");
        ultimoId++;
    }
    fecharArquivo(arquivo);
}

void listarProdutos(Produto *vetorProdutos, int quantidadeProdutos)
{
    printf("======= LISTAGEM DE PRODUTOS =======\n");
    int i;
    for (i = 0; i < quantidadeProdutos; i++)
    {
        printf("ID: %d, Nome: %s, Quantidade: %d, Valor: %.2f\n", vetorProdutos[i].id, vetorProdutos[i].nome, vetorProdutos[i].quantidade, vetorProdutos[i].valor);
    }
}

void excluirProduto()
{
    printf("Opção 4 - Excluir produto\n");

    int quantidadeProdutos, ultimoId;

    Produto *vetorProdutosListagem = carregarBancoDados(&quantidadeProdutos, &ultimoId);

    if (quantidadeProdutos == 0)
    {
        printf("Nenhum produto encontrado.");
        return;
    }

    printf("======= LISTAGEM DE PRODUTOS =======\n");

    listarProdutos(vetorProdutosListagem, ultimoId);

    printf("=====================================\n");

    printf("Digite o id do produto que gostaria de excluir:\n");

    int productId;
    scanf("%d", &productId);

    // Procura o produto pelo index fornecido
    int index = -1;
    int i;
    for (i = 0; i < quantidadeProdutos; i++)
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

    printf("Produto selecionado para exclusão:\n");
    printf("ID: %d, Nome: %s, Quantidade: %d, Valor: %.2f\n", vetorProdutosListagem[index].id, vetorProdutosListagem[index].nome, vetorProdutosListagem[index].quantidade, vetorProdutosListagem[index].valor);

    // Abre um arquivo temporário em modo de escrita para gravar os dados atualizados
    FILE *tempFile = abrirArquivo("temp_dados.txt", "w");

    if (tempFile == NULL)
    {
        printf("Erro ao abrir o arquivo temporário.\n");
        return;
    }

    // Grava os dados menos o do produto excluído
    for (i = 0; i < quantidadeProdutos; i++)
    {
        if (i != index)
        {
            fprintf(tempFile, "%d,%s,%d,%f;\n", vetorProdutosListagem[i].id, vetorProdutosListagem[i].nome, vetorProdutosListagem[i].quantidade, vetorProdutosListagem[i].valor);
        }
    }

    // Fecha o arquivo temporário
    fecharArquivo(tempFile);

    remove("dados.txt");                   // Deleta o arquivo original
    rename("temp_dados.txt", "dados.txt"); // Renomeia o arquivo temporário

    printf("Produto excluído com sucesso!\n");
}

void listarMenu()
{
    printf("\n=== Menu de Navegação ===\n");
    printf("1 - Cadastrar produto\n");
    printf("2 - Alterar produto\n");
    printf("3 - Excluir produto\n");
    printf("4 - Listar produtos\n");
    printf("0 - Sair\n");
}

void alterarProduto()
{
    printf("Opção 3 - Alterar produto\n");

    int quantidadeProdutos, ultimoId;

    Produto *vetorProdutosListagem = carregarBancoDados(&quantidadeProdutos, &ultimoId);

    if (quantidadeProdutos == 0)
    {
        printf("Nenhum produto encontrado.");
        return;
    }

    printf("======= LISTAGEM DE PRODUTOS =======\n");
    listarProdutos(vetorProdutosListagem, quantidadeProdutos);
    printf("=====================================\n");

    printf("Digite o id do produto que gostaria de alterar:\n");

    int productId;
    scanf("%d", &productId);

    // Procura o produto pelo index fornecido
    int index = -1;
    int i;
    for (i = 0; i < quantidadeProdutos; i++)
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

    // Pergunta qual atributo o usuario quer alterar
    printf("Informe qual o atributo que gostaria de alterar:\n");
    printf("1 - Nome\n");
    printf("2 - Quantidade\n");
    printf("3 - Valor\n");

    int escolha;
    scanf("%d", &escolha);

    // Captura e registra a alteração
    switch (escolha)
    {
    case 1:
        printf("Digite o novo nome do produto: ");
        getchar(); // Consume the newline character left in the buffer
        fgets(vetorProdutosListagem[index].nome, 50, stdin);
        strcpy(vetorProdutosListagem[index].nome, removeEspacos(vetorProdutosListagem[index].nome));
        break;
    case 2:
        printf("Digite a nova quantidade do produto no estoque: ");
        scanf("%d", &vetorProdutosListagem[index].quantidade);
        break;
    case 3:
        printf("Digite o novo valor de venda do produto: ");
        scanf("%f", &vetorProdutosListagem[index].valor);
        break;
    default:
        printf("Opção inválida. Nenhum atributo foi alterado.\n");
        return;
    }

    // Abre um arquivo temporário pra escrever os dados atualizados
    FILE *tempFile = abrirArquivo("temp_dados.txt", "w");

    if (tempFile == NULL)
    {
        printf("Erro ao abrir o arquivo temporário.\n");
        return;
    }

    // Insere os dados atualizados dentro do arquivo temporário
    for (i = 0; i < quantidadeProdutos; i++)
    {
        fprintf(tempFile, "%d,%s,%d,%f;\n", vetorProdutosListagem[i].id, vetorProdutosListagem[i].nome, vetorProdutosListagem[i].quantidade, vetorProdutosListagem[i].valor);
    }

    // Fecha o arquivo temporário
    fecharArquivo(tempFile);

    remove("dados.txt");                   // Apaga o arquivo original
    rename("temp_dados.txt", "dados.txt"); // Renomeia o arquivo temporário

    printf("Produto alterado com sucesso!\n");
}

int main()
{
    int ultimoId, quantidadeProdutos = 0;
    setlocale(LC_ALL, "");
    int opcao;

    do
    {
        // Exibindo o menu
        listarMenu();

        // Solicitando a escolha do usuario
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        // Executando a opcao escolhida
        switch (opcao)
        {
        case 1:
        {
            Produto *vetorProdutos = carregarBancoDados(&quantidadeProdutos, &ultimoId);
            cadastrarProduto(ultimoId);
            free(vetorProdutos);
            break;
        }
        case 2:
            alterarProduto();
            break;
        case 3:
            excluirProduto();
            break;
        case 4:
        {
            Produto *vetorProdutosListagem = carregarBancoDados(&quantidadeProdutos, &ultimoId);
            listarProdutos(vetorProdutosListagem, quantidadeProdutos);
            free(vetorProdutosListagem);
            break;
        }
        case 0:
            printf("Saindo do programa. Ate mais!\n");
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

/*
Autores: Vitor Arthur Keller e Lucas Camilo Moraes.
Curso: Engenharia de Software.

AVL Implementações:

Inserir número................[ok]
Remover número................[ok]
Pesquisar número..............[ok]
Pre-Order.....................[ok]
In-Order......................[ok]
Pos-order.....................[ok]
Mostrar Árvore AVL em gráfico.[ok]

*/

#include <stdio.h>
#include <stdlib.h>
#define tamanhoDoEspaco 6

typedef struct ITEM{
	int valor;
}tipoItem;

typedef struct NOH{
	tipoItem item;
	struct NOH *esquerda;
	struct NOH *direita;
	int fatorBalanceamento;
	int altura;
}tipoNoh;

tipoNoh *inicializar(){
	return NULL;
}

void imprimir(tipoNoh *raiz, int espaco){
    if (raiz == NULL){
        return;
    }else{
    espaco += tamanhoDoEspaco;
    imprimir(raiz->direita, espaco);
    printf("\n");
    
	for (int i = tamanhoDoEspaco; i < espaco; i++){
        printf(" ");
    }
    
    printf("%d\n", raiz->item.valor);
    imprimir(raiz->esquerda, espaco);
	}
}

void apagarArvore(tipoNoh *raiz){
	if (raiz != NULL){
		apagarArvore(raiz->esquerda);
		apagarArvore(raiz->direita);
		free(raiz);
		raiz = NULL;
	}
}

tipoNoh *pesquisar(tipoNoh *raiz, int valor){
	if (raiz != NULL){
		if (raiz->item.valor == valor){
			return raiz;
		}else{
			if (valor > raiz->item.valor){
			return pesquisar(raiz->direita, valor);
			}else{
			 return pesquisar(raiz->esquerda, valor);
			}
		}
	}
	
	return NULL;
}

tipoItem criarItem(int valor){
	tipoItem item;
	item.valor = valor;
	return item;
}

tipoNoh *alocar(){
	tipoNoh *novoNoh = (tipoNoh*) malloc(sizeof(tipoNoh));
	
	if (novoNoh != NULL){
		return novoNoh;
	}else{
		printf("Erro ao criar novo no\n");
		exit(1);
	}
}

int alturaNoh(tipoNoh *raiz){
    if (raiz==NULL){
        return -1;
	}else{
        return raiz->altura;
	}
}

int fatorBalanceamento(tipoNoh *raiz){
    return labs(alturaNoh(raiz->esquerda)-alturaNoh(raiz->direita));
}

int alturaArvore(tipoNoh *raiz){
	int alturaDireita=0;
	int alturaEsquerda=0;

	if (raiz == NULL){
		return -1;
	}
	
	alturaEsquerda = alturaArvore(raiz->esquerda);
	alturaDireita = alturaArvore(raiz->direita);

	if (alturaEsquerda > alturaDireita){
		return alturaEsquerda+1;
	}else{
		return alturaDireita+1;
	}
}

int contarNos(tipoNoh *raiz){
	if (raiz == NULL){
		return 0;
	}else{
		return contarNos(raiz->esquerda) + contarNos(raiz->direita) + 1;
	}
}

int maior(int x, int y){
	if (x > y){
		return x;
	}else{
		return y;
	}
}

void rotacaoDireita(tipoNoh **raiz){
	tipoNoh *nohAuxiliar;
	nohAuxiliar  = (*raiz)->esquerda;
	(*raiz)->esquerda  = nohAuxiliar->direita;
	nohAuxiliar->direita = *raiz;
	
	(*raiz)->altura = maior(alturaNoh((*raiz)->esquerda), alturaNoh((*raiz)->direita))+1;
	nohAuxiliar->altura = maior (alturaNoh(nohAuxiliar->esquerda), (*raiz)->altura)+1;

	*raiz = nohAuxiliar;
}

void rotacaoEsquerda(tipoNoh **raiz){
	tipoNoh *nohAuxiliar;
	nohAuxiliar  = (*raiz)->direita;
	(*raiz)->direita  = nohAuxiliar->esquerda;
	nohAuxiliar->esquerda = *raiz;

	(*raiz)->altura = maior(alturaNoh((*raiz)->esquerda), alturaNoh((*raiz)->direita))+1;
	nohAuxiliar->altura = maior (alturaNoh(nohAuxiliar->esquerda), (*raiz)->altura)+1;

	*raiz = nohAuxiliar;
}

void rotacaoEsquerdaDireita(tipoNoh **raiz){
	rotacaoEsquerda(&(*raiz)->esquerda);
	rotacaoDireita(raiz);
}

void rotacaoDireitaEsquerda(tipoNoh **raiz){
	rotacaoDireita(&(*raiz)->direita);
	rotacaoEsquerda(raiz);
}

tipoNoh* procuraMenor(tipoNoh **raiz){
    tipoNoh *nohAuxiliar1;
    tipoNoh *nohAuxiliar2;

    nohAuxiliar1 = (*raiz);
    nohAuxiliar2 = (*raiz)->esquerda;

    while(nohAuxiliar2!=NULL){
        nohAuxiliar1 = nohAuxiliar2;
        nohAuxiliar2 = nohAuxiliar2->esquerda;
    }
    
    return nohAuxiliar1;
}

int inserir(tipoNoh **raiz, int valor){
    int controle;

	if (*raiz == NULL){
		tipoNoh *nohAuxiliar = alocar();
		nohAuxiliar->item.valor = valor;
		nohAuxiliar->altura = 0;
		nohAuxiliar->esquerda  = NULL;
		nohAuxiliar->direita = NULL;
        *raiz = nohAuxiliar;
		return 1;
	}

	tipoNoh *nohAtual;
	nohAtual = *raiz;

    if(valor < nohAtual->item.valor){
        if((controle=inserir(&(nohAtual->esquerda),valor))==1){
            if(fatorBalanceamento(nohAtual)>=2){
                if(valor < (*raiz)->esquerda->item.valor){
                    rotacaoDireita(raiz);
				}else{
                    rotacaoEsquerdaDireita(raiz);
				}
            }
        }
    }else{
        if(valor > nohAtual->item.valor){
            if ((controle=inserir(&(nohAtual->direita),valor))==1){
                if(fatorBalanceamento(nohAtual)>=2){
                    if(valor > (*raiz)->direita->item.valor){
                        rotacaoEsquerda(raiz);
                    }else{
                        rotacaoDireitaEsquerda(raiz);
					}
                }
            }
        }else{
            printf("\nValor Duplicado!\n");
            return 0;
        }
    }
    
    nohAtual->altura = maior(alturaNoh(nohAtual->esquerda),alturaNoh(nohAtual->direita))+1;
    return controle;
}

int remover(tipoNoh **raiz, int valor){
    int controle;

    if (*raiz == NULL){
        printf("\nEste valor nao existe\n");
        return 0;
    }
    
    if (valor < (*raiz)->item.valor){
        if ((controle = remover(&((*raiz)->esquerda), valor)) == 1){
            if (fatorBalanceamento(*raiz) >= 2){
                if (alturaNoh((*raiz)->direita->esquerda) <= alturaNoh((*raiz)->direita->direita)){
                    rotacaoEsquerda(raiz);
                }else{
                    rotacaoDireitaEsquerda(raiz);
                }
            }
        }
    }else if(valor > (*raiz)->item.valor){
        if ((controle = remover(&((*raiz)->direita), valor)) == 1){
            if (fatorBalanceamento(*raiz) >= 2){
                if (alturaNoh((*raiz)->esquerda->direita) <= alturaNoh((*raiz)->esquerda->esquerda)){
                    rotacaoDireita(raiz);
                }else{
                    rotacaoEsquerdaDireita(raiz);
                }
            }
        }
    }else{
        if (((*raiz)->esquerda == NULL) || ((*raiz)->direita == NULL)){
            tipoNoh *nohAuxiliar = *raiz;
            if ((*raiz)->esquerda != NULL){
                *raiz = (*raiz)->esquerda;
            }else{
                *raiz = (*raiz)->direita;
            }
            free(nohAuxiliar);
            printf("\nNo removido com sucesso!\n");
        }else{
            tipoNoh *nohAuxiliar = procuraMenor(&(*raiz)->direita);
            (*raiz)->item.valor = nohAuxiliar->item.valor;
            remover(&(*raiz)->direita, nohAuxiliar->item.valor);
        }
	}

    if (*raiz != NULL){
        (*raiz)->altura = maior(alturaNoh((*raiz)->esquerda), alturaNoh((*raiz)->direita)) + 1;
        if (fatorBalanceamento(*raiz) >= 2){
            if (alturaNoh((*raiz)->esquerda->direita) <= alturaNoh((*raiz)->esquerda->esquerda)){
                rotacaoDireita(raiz);
            }else{
                rotacaoEsquerdaDireita(raiz);
            }
        }else if (fatorBalanceamento(*raiz) <= -2){
            if (alturaNoh((*raiz)->direita->esquerda) <= alturaNoh((*raiz)->direita->direita)){
                rotacaoEsquerda(raiz);
            }else{
                rotacaoDireitaEsquerda(raiz);
            }
        }
    }
    
    return 1;
}

void preOrder(tipoNoh *raiz){
    if (raiz != NULL){
        printf("%d ", raiz->item.valor);
        preOrder(raiz->esquerda);
        preOrder(raiz->direita);
    }
}

void inOrder(tipoNoh *raiz){
    if (raiz != NULL){
        inOrder(raiz->esquerda);
        printf("%d ", raiz->item.valor);
        inOrder(raiz->direita);
    }
}

void posOrder(tipoNoh *raiz){
    if (raiz != NULL){
        posOrder(raiz->esquerda);
        posOrder(raiz->direita);
        printf("%d ", raiz->item.valor);
    }
}

int main(void){
    tipoNoh *raiz = inicializar();
    
    // Função para inserir números iniciais na arvóre AVL
    //int insercoesIniciais[] = {50, 57, 45, 25, 44, 27, 12, 55, 80, 95};
    //int tamanho = sizeof(insercoesIniciais) / sizeof(insercoesIniciais[0]);

    //for (int i = 0; i < tamanho; i++){
    //    inserir(&raiz, insercoesIniciais[i]);
    //}

    int opcao = -1;
    int valor = 0;

    while (opcao != 0){
        printf("\n========== Menu ==========\n");
        printf("1: Inserir numero\n");
        printf("2: Remover numero\n");
        printf("3: Pesquisar numero\n");
        printf("4: Mostrar percursos em order\n");
        printf("5: Mostrar Arvore AVL\n");
        printf("0: Sair\n");
        printf("Digite uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("\n===== Inserindo =====\n");
                printf("Digite um valor: ");
                scanf("%d", &valor);
                inserir(&raiz, valor);
                break;

            case 2:
                printf("\n===== Removendo =====\n");
                printf("Digite um valor: ");
                scanf("%d", &valor);
                remover(&raiz, valor);
                break;

            case 3:
                printf("\n===== Pesquisando =====\n");
                printf("Digite um valor: ");
                scanf("%d", &valor);
                
                tipoNoh *nohAuxiliar = pesquisar(raiz, valor);
                if (nohAuxiliar == NULL) {
                    printf("\nElemento nao encontrado!\n");
                } else {
                    printf("\nElemento encontrado com sucesso!\n");
                }
                break;

            case 4:
                printf("\n===== Persursos =====\n");
                printf("Pre-order: ");
                preOrder(raiz);
                printf("\nIn-order: ");
                inOrder(raiz);
                printf("\nPos-order: ");
                posOrder(raiz);
                printf("\n");
                break;

            case 5:
                printf("\n==================== Arvore AVL ====================\n");
                imprimir(raiz, tamanhoDoEspaco);
                printf("\n");
                break;

            case 0:
                printf("\nSaindo...\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
                break;
        }
    }
    
    apagarArvore(raiz);
    return 0;
}
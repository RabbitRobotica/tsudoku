#include <stdio.h>
#include <time.h>
 
//#define DEBUG
 
#ifdef DEBUG
	#define DPRINTF(fmt, args...) fprintf(stderr,"%s %s %d " fmt,__FILE__, __FUNCTION__,__LINE__, ##args )
#else
	#define DPRINTF(fmt, args...) 
#endif
 
#define TAM 9 //melhor criar esse define para organizar o codigo
 
#define ALLBIT 1022 //1022 = 1111111110 binario
 
#define MAXJOG (9*9)
 
#define MAXNIV 80
 
//--- MACROS PARA MANIPULAR BITS:
#define LIGA_BIT(NUM,BIT)  (NUM |= (1<<(BIT)))
 
#define APAGA_BIT(NUM,BIT) (NUM &= (~(1<<(BIT))))
 
#define TESTA_BIT(NUM,BIT) (NUM & (1<<(BIT)))
 
int resolvido[TAM][TAM]=
{
7,9,3,5,6,8,2,4,1,
4,5,1,7,2,9,3,6,8,
6,8,2,3,1,4,9,7,5,
1,6,5,4,9,3,7,8,2,
2,7,8,1,5,6,4,3,9,
3,4,9,2,8,7,5,1,6,
9,3,6,8,7,5,1,2,4,
5,1,4,6,3,2,8,9,7,
8,2,7,9,4,1,6,5,3,
};
 
static int resolvido_ok;
 
int sudoku[TAM][TAM]=
{
	0,0,0,0,6,0,2,4,0,
	4,5,0,0,2,0,3,0,0,
	0,8,0,3,0,0,0,0,0,
 
	0,0,5,4,0,0,0,0,0,
	0,7,8,0,5,0,0,0,9,
	0,0,9,2,0,0,0,0,0,
 
	0,3,0,8,0,0,0,0,0,
	5,1,0,0,3,0,8,0,0,
	0,0,0,0,4,0,6,5,0,
};
 
/*
int sudoku[TAM][TAM]=
{
	0,0,6,4,0,0,0,0,0,
	3,5,0,0,7,0,0,0,0,
	4,0,0,6,0,3,0,0,0,
 
	0,3,0,0,0,0,1,0,8,
	5,0,2,0,6,0,4,0,9,
	6,0,1,0,0,0,0,2,0,
 
	0,0,0,7,0,8,0,0,1,
	0,0,0,0,3,0,0,4,5,
	0,0,0,0,0,5,8,0,0,
};
*/
 
 
int NumSetBits( int num ) //Numero de bits ligados no inteiro
{
	int i, res;
 
	for (res=0,i=0;i<32;i++)
	{
		if ( TESTA_BIT(num,i) )
		{
			res++;
		}
	}
	return res;
}
 
 
void MostraSudoku()
{
	int i, j;
 
	for (i=0;i<TAM;i++)
	{
		if (i && (i%3==0)) printf("-------------------\n");
		for (j=0;j<TAM;j++)
		{
			if (j && (j%3==0)) printf("|");
 
			if ( sudoku[i][j]==0 )
			{
				printf(". ");	
			}
			else
			{
				printf("%d ", sudoku[i][j] );
			}
 
		}
		printf("\n");
	}
	printf("\n");
 
}
 
 
 
 
 
int ResolveSudoku( int nivel, int x, int y, int val, int mostra )
{
	int i, j, k, m, mask;
	int quad_i, quad_j;
 
 
	int melhor_x, melhor_y, melhor_mask, melhor_bits, bits;
 
	if (resolvido_ok)
	{
		return 0;
	}
 
	if ( val>=1&& val<=9 )
	{
 
		sudoku[x][y]=val; //executa a jogada
	}
 
	#ifdef DEBUG
	printf("nivel=%d\n",nivel);
	MostraSudoku();
	getchar();
	#endif
 
	resolvido_ok=1;
	melhor_bits=0;
	melhor_x=-1;
 
	for (i=0;i<TAM;i++)
	{
		for (j=0;j<TAM;j++)
		{
			//para cada casa do tabuleiro q esta vazia
			if ( sudoku[i][j]==0 )
			{
				mask=0;
				resolvido_ok=0;
				//pesquisa linha
				for ( k=0; k<TAM; k++ )
				{
					if (sudoku[i][k]!=0)
					{
						LIGA_BIT(mask,sudoku[i][k]);//liga os bits q indicam numeros impossiveis
					}
				}
 
				//pesquisa coluna
				for ( k=0; k<TAM; k++ )
				{
					if (sudoku[k][j]!=0)
					{
						LIGA_BIT(mask,sudoku[k][j]);//liga os bits q indicam numeros impossiveis
					}
				}
				//pesquisa quadrado
 
				quad_i = (i/3)*3;
				quad_j = (j/3)*3;
				DPRINTF("quad_i=%d quad_j=%d\n",quad_i,quad_j);
				for (k=quad_i; k<quad_i+3;k++)
				{
					for (m=quad_j;m<quad_j+3;m++)
					{
						if (sudoku[k][m]!=0)
						{
							LIGA_BIT(mask,sudoku[k][m]); //liga os bits q indicam numeros impossiveis	
						}
					}
				}
 
				if (mask==ALLBIT) //1022 = binario 1111111110
				{
					//Se nao existe possibilidade de jogo ocorreu erro atras...
 
					#ifdef DEBUG 
					printf("SUDOKU ERRADO: nivel=%d i=%d j=%d\n",nivel,i,j);
					MostraSudoku();
					getchar();
					#endif
 
					goto fim_loop;
 
 
 
				}
				DPRINTF("mask[%d][%d]=%d\n",i,j,mask);
 
				if ( ( bits=NumSetBits(mask) ) > melhor_bits )
				{
					melhor_bits=bits;
					melhor_x=i;
					melhor_y=j;
					melhor_mask=mask;
				}
 
			}
		}
	}
 
 
	if (resolvido_ok)
	{
		if (mostra)//se eh para mostrar a solução...
		{	
			printf("SUDOKU RESOLVIDO: nivel=%d\n",nivel);
			MostraSudoku();
		//	getchar();
		}
		return 0;
 
	}
 
	//executa a melhor jogada encontrada...
	if (melhor_x!=-1)
	for (k=1;k<=9;k++)
	{
		if ( TESTA_BIT(melhor_mask,k)==0 )
		{
			ResolveSudoku(nivel+1,melhor_x,melhor_y,k,mostra);
		}
	}
 
 
	fim_loop:
 
	if (val>=1&&val<=9)
	{
		sudoku[x][y]=0; //undo da jogada
 
	}
 
 
	return 1;
}
 
void GeraSudoku()
{
	int i, j, k, nivel;
	char letra;
 
 
	do
	{
		printf("Digite o nivel do sudoku: (0-infatil 1-facil 2-medio 3-dificil 4-louco)\n");
		letra=getchar();
		setbuf(stdin,0);
 
 
	}while ( letra<'0'||letra>'4');
 
	switch (letra)
	{
		case '0': nivel=49; break;
		case '1': nivel=40; break;
		case '2': nivel=31; break;
		case '3': nivel=25; break;
		case '4': nivel=20; break;
		default:nivel=31; break; 
	}
 
	k=0;
 
	do
	{
		memset(sudoku,0,sizeof(int)*TAM*TAM);
 
		for (i=0;i<TAM;i++)
		{
			for (j=0;j<TAM;j++)
			{
				if ( rand() % 100 < nivel+k )
				{
					sudoku[i][j]=resolvido[i][j];
				}
			}
		}
 
		k++;
 
	} while ( ResolveSudoku(0,0,0,0,0) );
 
 
	printf("Gerado sudoku nivel (quanto mais baixo mais dificil)=%d\n",nivel+k-1);
 
	MostraSudoku();
}
 
int main()
{
 
 
// 	MostraSudoku();
 
//	ResolveSudoku(0,0,0,0,1);
 
	char resp;
 
	srand(time(NULL));
 
	GeraSudoku();
 
	printf("Deseja ver a solucao? (SN)\n");
 
	resp=getch();
 
 
	if (resp=='s'||resp=='S')
	{
		resolvido_ok=0;
		ResolveSudoku(0,0,0,0,1);
	}
 
 
	return 0;
}

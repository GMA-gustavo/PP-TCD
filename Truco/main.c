

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#define MAX 128 // definindo tamanho max executando TRUCO
#ifdef WIN32//para limpar o terminal nao importando o SO
    #include <conio.h>
    #include <windows.h>
    #define LIMPA_TELA system("cls");
#else
  #include <gconio.h>
  #define LIMPA_TELA system("clear");

#endif

#include <unistd.h>//funcao sleep



struct carta{
    char valor;
    char naipe;
    int forca;
    int jogada; //se a carta j� foi utilizada, jogada = 1, se ela n�o foi utilizda jogada = 0
};

struct jogador{
    char nome[20];
    int vitorias;
    int derrotas;
    int pontos;
    int pontosM;
    struct carta cartas[3];
};


int jogador(int cp1, struct jogador *p1, int maoDeFerro);
int bot(int cp2);
int trucoP1(int *valMao);
int trucoP2(struct carta cartas[3], int rodada, int *valMao);                                    //prototipagem das nossas fun��es
int check(char *str1,char *str2,int len1,int len2);
void separar(char *str,int *vit,int *derr);
void buscarJogador(struct jogador *p1);
void escreverJogador(char *strNome,int vit,int derr);
void mout(char *nome,int lenNome,int vit,int derr,FILE *p,int achou);
int maoDeOnzeJogador(int *valMao,int *acaba, struct jogador *p1);
int maoDeOnzeBot(int *valMao, int *acaba, struct carta cartas[3]);
            // Fun��es do bot
void organiza (struct carta *cartas1, struct carta *cartas2, struct carta *cartas3);
double numero (struct carta cartas[3]);
int combinacao(double p);
int randomizar(double p);
int jogadas (struct carta cartas[3], int PontosBot, int rodada, int PesoCartaPlayer, int PesoRodada, int *JogadasBot, int cangada, int *PrimeiraRodada);

//nicolas adicionando
void Carta(char valor, char naipe, int op, int jogada, int forca);//printar as cartas
void printar_imagem(FILE *fptr);//printar os arquivos de imagem: TRUCO e BANNER
void printar_logo(FILE *arquivologo);//em cima ta explicado
void cartaMesa(char valor, char naipe, int forca);//cartas jogadas na rodada
int jogadorMaodeFerro(int cp1, struct jogador *p1);// para printar as cartas escondidas da mao de ferro
int carta_virada(int op, int jogada);//printar carta virada






int main(){
    setlocale(LC_ALL, "Portuguese");

    srand(time(NULL));
    struct carta baralho[40];
    struct jogador p1,p2;
    char naipes[4] = {'O','C','E','P'};
    char valores[10] ={'3','2','A','K','J','Q','7','6','5','4'};
    int kas[6];
    int i,j,k,l,aprove;  //Vari�veis utilizadas para fazer a randomiza��o do baralho e a distribui��o das cartas
    int changePlayer,changePlayerglobal=1,turn; // quando turn eh igula a zero o bot come�a, quando eh igual a 1 o jogador come�a // turn se refere a qual rodada da m�o eest� sendo disputada
    int cp1,cp2; // numeros relativos �s cartas jogadas por cada player
    int acaba; //dita se a rodada de m�o ir� acabar ou n�o
    char vitorias[3];  // vetor sobre quem ganhou cada rodada da m�o
    int maoDeOnze,maoDeFerro;
    int valMao; // referente ao valor da m�o 1,3,6,9,12
    int reTruco; // referente ao retorno das fun��es truco
    int entMenu; //referente a entrada no menu de op��es
    int entFim; //referente a entrada do menu que aparece no final do jogo;
    int ArrJogadasBot[3];
    int cangada;
    int PrimeiraRodada = 1;

    FILE *rank;
    char *fileR;
    char str[20];


    for(i=0,k=0;i<4;i++){
        for(j=0,l=10;j<10;j++,k++,l--){
            baralho[k].naipe = naipes[i];
            baralho[k].valor = valores[j];
            baralho[k].forca = l;
        }
    }

    baralho[39].forca = 14; // zap
    baralho[16].forca = 13; // sete copas
    baralho[22].forca = 12; // as de espadas
    baralho[6].forca = 11; // sete ouros


    do{
        do{
            char *filename = "TRUCO.txt";
            FILE *fptr = NULL;

            if((fptr = fopen(filename,"r")) == NULL)
            {
                fprintf(stderr,"erro ao abrir %s\n",filename);
                return 1;
        }
            printar_imagem(fptr);

            fclose(fptr);
            printf("||==========================================================||\n");
            printf("||                          MENU                            ||\n");
            printf("||==========================================================||\n");
            printf("|| Op��es:                                                  ||\n");
            printf("|| 1 - Jogar                                                ||\n");
            printf("|| 2 - Ranking                                              ||\n");
            printf("|| 3 - Sobre                                                ||\n");
            printf("|| 4 - Sair                                                 ||\n");
            printf("||==========================================================||\n");
            printf("||                                                          ||\n");
            printf("||==========================================================||\n");
            printf("|| Digite a op��o desejada abaixo                           ||\n");
            printf("||==========================================================||\n");
            printf("-> ");
            scanf("%d", &entMenu);
            if(!(entMenu==1||entMenu==2||entMenu==3||entMenu==4)){
                printf("Op��o inv�lida, digite novamente\n->");
            }
        }while(!(entMenu==1||entMenu==2||entMenu==3||entMenu==4));





        if(entMenu==1){
            //Op��o jogar

            LIMPA_TELA;
            printf("Op��o selecionada: [1] - Jogar\n");

            buscarJogador(&p1);
            printf("\n%s  ->   Vitorias: %d   Derrotas: %d\n\n",p1.nome,p1.vitorias,p1.derrotas);
            p1.pontos = 0;
            p2.pontos = 0;
            sleep(1);

            LIMPA_TELA;
            printf("||===================||\n");
            printf("|| EMBARALHANDO..... ||\n");
            printf("||===================||\n");
            sleep(1.5);
            LIMPA_TELA;

            do{
                while(p1.pontos<12 && p2.pontos<12){
                    p1.pontosM = 0;
                    p2.pontosM =0;
                    acaba = 0;   // 1-> a m�o acaba 0 -> a m�o continua
                    turn =0;
                    valMao = 1;
                    maoDeOnze = 0;
                    maoDeFerro = 0;

                    for(i=0;i<6;i++){
                    do{
                    aprove = 0;
                    k = rand() % 39;
                    for(j=0;j<6;j++){
                        if(kas[j]==k){
                            aprove = 1;
                        }
                    }
                    }while(aprove);
                    kas[i] = k;
                    if(i<3){
                        p1.cartas[i].naipe = baralho[k].naipe;
                        p1.cartas[i].valor = baralho[k].valor;
                        p1.cartas[i].forca = baralho[k].forca;
                        p1.cartas[i].jogada = 1;
                    }
                    else{
                        p2.cartas[i-3].naipe = baralho[k].naipe;
                        p2.cartas[i-3].valor = baralho[k].valor;
                        p2.cartas[i-3].forca = baralho[k].forca;
                        p2.cartas[i-3].jogada = 1;
                    }
                    }


                    if(p1.pontos==11&&p2.pontos==11){
                        printf("Na m�o de ferro ningu�m pode ver a suas cartas, escolha alguma carta e deixe a sorte decidir o seu destino\n");
                        maoDeFerro = 1;
                    }
                    else{

                        if(p1.pontos==11){
                            if(!maoDeOnzeJogador(&valMao,&acaba, &p1)){
                                p2.pontos+=1;
                            }
                            maoDeOnze = 1;
                        }
                        else if(p2.pontos==11){
                            if(!maoDeOnzeBot(&valMao, &acaba, p2.cartas)){
                                p1.pontos+=1;
                            }
                            maoDeOnze = 1;
                        }
                        organiza(&p2.cartas[0],&p2.cartas[1],&p2.cartas[2]);
                    }




                    changePlayerglobal = changePlayerglobal == 0 ? 1:0;
                    changePlayer = changePlayerglobal;
                    cangada = 0;

                    while(!acaba){        //mao



                        if(changePlayer){ //jogador come�a
                            reTruco = 0;
                            cp1 = 0;
                            cp1 = jogador(cp1,&p1,maoDeFerro);
                            if(cp1==4){
                                if(maoDeOnze){
                                    acaba = 1;
                                    p2.pontos+=12-p2.pontos;
                                }
                                else{
                                    do{
                                        reTruco = trucoP2(p2.cartas, turn, &valMao);
                                        if(reTruco==1){
                                            printf("##################\n");
                                            printf("O bot aceitou\n");
                                        printf("##################\n");

                                            cp1 = jogador(cp1,&p1, maoDeFerro);

                                            sleep(3);
                                        }
                                        else if(reTruco==0){
                                            printf("##################\n");
                                            printf("O bot n�o aceitou o truco\n");
                                            printf("##################\n");
                                            sleep(2.5);
                                            acaba = 1;
                                            p1.pontos+=valMao;
                                        }
                                        else if(reTruco == 2){
                                            reTruco = trucoP1(&valMao);
                                            if(reTruco == 1){

                                                cp1 = jogador(cp1,&p1, maoDeFerro);
                                            }
                                            else if(reTruco==0){
                                                printf("##################\n");
                                                printf("O player n�o aceitou o truco\n");
                                            printf("##################\n");
                                            sleep(2.5);
                                                acaba = 1;
                                                p2.pontos+=valMao;
                                            }

                                        }
                                    }while(reTruco==2);
                                }
                            }
                            if(acaba==0){
                                cp2 = 0;
                                if(maoDeFerro){
                                    cp2 = turn;
                                }
                                else{
                                    cp2 = jogadas(p2.cartas, p2.pontos, turn, p1.cartas[cp1].forca,valMao,ArrJogadasBot,cangada, &PrimeiraRodada);
                                    if(cp2==4){
                                        if(maoDeOnze){
                                            acaba=1;
                                            p1.pontos+=12-p1.pontos;
                                        }
                                        else{
                                            do{
                                            reTruco = trucoP1(&valMao);
                                            if(reTruco == 1){
                                                cp2 = jogadas(p2.cartas, p2.pontos, turn, p1.cartas[cp1].forca,valMao,ArrJogadasBot,cangada, &PrimeiraRodada);
                                            }
                                            else if(reTruco==0){
                                                    printf("##################\n");
                                                printf("O player n�o aceitou o truco\n");
                                            printf("##################\n");
                                            sleep(2.5);
                                                acaba = 1;
                                                p2.pontos+=valMao;
                                            }
                                            else if(reTruco==2){
                                                reTruco = trucoP2(p2.cartas, turn, &valMao);
                                                if(reTruco==1){
                                                    printf("##################\n");
                                                    printf("O bot aceitou!\n");
                                                printf("##################\n");
                                                    cp2 = jogadas(p2.cartas, p2.pontos, turn, p1.cartas[cp1].forca,valMao,ArrJogadasBot,cangada, &PrimeiraRodada);

                                                    sleep(3);
                                                }
                                                else if(reTruco==0){
                                                    printf("##################\n");
                                                    printf("O bot n�o aceitou o truco\n");
                                                    printf("##################\n");
                                                    sleep(2.5);
                                                    acaba = 1;
                                                    p1.pontos+=valMao;
                                                }
                                            }
                                            }while(reTruco==2);
                                        }
                                    }
                                    //print de cartas
                                    printf("\n=============\n");
                                    printf("Carta do Bot:\n");
                                    cartaMesa(p2.cartas[cp2].valor, p2.cartas[cp2].naipe, p2.cartas[cp2].forca);
                                    printf("=============\n");
                                    sleep(3);
                                }
                            }
                        }
                        else{ //bot come�a
                            reTruco = 0;
                            cp2 = 0;
                            if(maoDeFerro){
                                cp2 = turn;
                            }
                            else{
                                cp2 = jogadas(p2.cartas, p2.pontos, turn, 0,valMao,ArrJogadasBot,cangada, &PrimeiraRodada);
                                if(cp2==4){
                                    if(maoDeOnze){
                                        acaba=1;
                                        p1.pontos+=12-p1.pontos;
                                    }
                                    else{
                                        do{
                                            reTruco = trucoP1(&valMao);
                                            if(reTruco == 1){
                                                cp2 = jogadas(p2.cartas, p2.pontos, turn, 0,valMao,ArrJogadasBot,cangada, &PrimeiraRodada);
                                            }
                                            else if(reTruco==0){
                                                printf("##################\n");
                                                printf("O player n�o aceitou o truco\n");
                                            printf("##################\n");
                                            sleep(2.5);
                                                acaba = 1;
                                                p2.pontos+=valMao;
                                            }
                                            else if(reTruco==2){
                                                reTruco = trucoP2(p2.cartas, turn, &valMao);
                                                if(reTruco==1){
                                                    printf("##################\n");
                                                    printf("O bot aceitou!\n");
                                                printf("##################\n");
                                                    cp2 = jogadas(p2.cartas, p2.pontos, turn, 0,valMao,ArrJogadasBot,cangada, &PrimeiraRodada);

                                                    sleep(3);
                                                }
                                                else if(reTruco==0){
                                                        printf("##################\n");
                                                    printf("O bot n�o aceitou o truco\n");
                                                printf("##################\n");
                                                sleep(2.5);
                                                    acaba = 1;
                                                    p1.pontos+=valMao;
                                                }
                                            }
                                        }while(reTruco==2);
                                    }
                                }
                            }
                            printf("\n=============\n");
                            printf("Carta do Bot:\n");
                            cartaMesa(p2.cartas[cp2].valor, p2.cartas[cp2].naipe, p2.cartas[cp2].forca);
                            printf("=============\n");

                            sleep(3);
                            if(acaba==0){
                                cp1 = 0;
                                cp1 = jogador(cp1,&p1, maoDeFerro);
                                if(cp1==4){
                                    if(maoDeOnze){
                                        acaba = 1;
                                        p2.pontos+=12-p2.pontos;;
                                    }
                                    else{
                                        do{
                                            reTruco = trucoP2(p2.cartas, turn, &valMao);
                                            if(reTruco==1){
                                                printf("##################\n");
                                                printf("O bot aceitou!\n");
                                            printf("##################\n");
                                                cp1 = jogador(cp1,&p1, maoDeFerro);
                                                sleep(3);
                                            }
                                            else if(reTruco==0){

                                                printf("##################\n");
                                                printf("O bot n�o aceitou o truco\n");
                                                printf("##################\n");sleep(2.5);
                                                acaba = 1;
                                                p1.pontos+=valMao;
                                            }
                                            else if(reTruco == 2){
                                                reTruco = trucoP1(&valMao);
                                                if(reTruco == 1){
                                                    cp1 = jogador(cp1,&p1, maoDeFerro);
                                                }
                                                else if(reTruco==0){
                                                    printf("##################\n");
                                                    printf("O player n�o aceitou o truco\n");
                                                printf("##################\n");sleep(2.5);
                                                    acaba = 1;
                                                    p2.pontos+=valMao;
                                                }

                                            }
                                        }while(reTruco==2);
                                    }
                                }
                            }
                        }


                        if(acaba == 0 &&p1.cartas[cp1].forca > p2.cartas[cp2].forca){
                            LIMPA_TELA;
                            printf("=================Rodada do jogador================\n");
                            printf("==========Cartas Jogadas na rodadas anterior======\n");
                            printf("%s jogou: \n", p1.nome);
                            cartaMesa(p1.cartas[cp1].valor, p1.cartas[cp1].naipe, p1.cartas[cp1].forca);
                            printf("Maquina jogou: \n");
                            cartaMesa(p2.cartas[cp2].valor, p2.cartas[cp2].naipe,p2.cartas[cp2].forca);
                            printf("===================================================\n");
                            sleep(5);
                            LIMPA_TELA;
                            p1.cartas[cp1].jogada = 0;
                            p2.cartas[cp2].jogada = 0;
                            p1.pontosM++;
                            vitorias[turn] = 'J';
                            changePlayer = 1;
                        }
                            if(acaba == 0 && p1.cartas[cp1].forca == p2.cartas[cp2].forca){
                            LIMPA_TELA;
                            p1.cartas[cp1].jogada = 0;
                            p2.cartas[cp2].jogada = 0;
                            printf("==================Rodada Cangada==================\n");
                            printf("========Cartas Jogadas na rodadas anterior========\n");
                            printf("+-+-+-Cartas com for�as iguais foram jogadas-+-+-+: \n");
                            printf("%s jogou: \n", p1.nome);
                            cartaMesa(p1.cartas[cp1].valor, p1.cartas[cp1].naipe, p1.cartas[cp1].forca);
                            printf("Maquina jogou:\n");
                            cartaMesa(p2.cartas[cp2].valor, p2.cartas[cp2].naipe, p2.cartas[cp2].forca);
                            printf("===================================================\n");
                            sleep(5);
                            LIMPA_TELA;
                            vitorias[turn] = 'C';
                            cangada = 1;
                            if(turn==0){
                                p1.pontosM++;
                                p2.pontosM++;
                            }
                            else{
                                switch(vitorias[0]){
                                    case 'J':
                                        p1.pontosM++;
                                    break;
                                    case 'B':
                                        p2.pontosM++;
                                    break;
                                    case 'C':
                                        if(turn==2&&vitorias[1]=='C'){
                                            acaba = 0;
                                            printf("Todas as rodadas foram cangadas, ningu�m ganhar� o tento");
                                        }
                                    break;
                                }
                            }
                        }
                            if(acaba == 0 && p1.cartas[cp1].forca < p2.cartas[cp2].forca){
                            LIMPA_TELA;

                            printf("=================Rodada da Maquina================\n");
                            printf("==========Cartas Jogadas na rodadas anterior======\n");
                            printf("Maquina jogou: \n");
                            cartaMesa(p2.cartas[cp2].valor, p2.cartas[cp2].naipe, p2.cartas[cp2].forca);
                            printf("%s jogou: \n", p1.nome);
                            cartaMesa(p1.cartas[cp1].valor, p1.cartas[cp1].naipe, p1.cartas[cp1].forca);
                            printf("===================================================\n");
                            sleep(5);
                            LIMPA_TELA;
                            p1.cartas[cp1].jogada = 0;
                            p2.cartas[cp2].jogada = 0;
                            p2.pontosM++;
                            vitorias[turn] = 'B';
                            changePlayer = 0;
                        }

                        if(p1.pontosM == 2){
                            //mao acaba, jogador 1 recebe um ponto
                            if(p1.pontos+valMao>=12){
                                p1.pontos = 12;
                            }
                            else{
                                p1.pontos+=valMao;
                            }
                            acaba = 1;
                        }
                        if(p2.pontosM == 2){
                            if(p2.pontos+valMao>=12){
                                p2.pontos = 12;
                            }
                            else{
                                p2.pontos+=valMao;
                            }
                            acaba = 1;
                            //mao acaba, jogador 2 recebe um ponto
                        }
                        turn++;
                    }

                    printf("||========================================================================||\n");
                    printf("||         A m�o acabou, pontos do jogador: %d,  pontos do bot: %d        ||\n",p1.pontos,p2.pontos);
                    printf("||========================================================================||\n");
                    sleep(3.5);
                    LIMPA_TELA;
                }



                if(p1.pontos>p2.pontos){
                    printf("||========================================================================||\n");
                    printf("||                    VOC� VENCEU! VOA MOLEQUE!!! :-D                     ||\n");
                    printf("||========================================================================||\n");
                    p1.vitorias++;
                    //escrever no arquivo aumentando um no numero de derrotas
                }
                else{
                    printf("||========================================================================||\n");
                    printf("||                   Vish... mais sorte na proxima :-(                    ||\n");
                    printf("||========================================================================||\n");
                    p1.derrotas++;
                    //escrever no arquivo aumentando um no numero de derrotas
                }


                escreverJogador(p1.nome,p1.vitorias,p1.derrotas);

                do{
                    printf("||========================================================================||\n");
                    printf("|| Jogo finalizado! Pontua��o do jogador: %d,  Pontua��o do bot: %d       ||\n",p1.pontos,p2.pontos);
                    printf("||========================================================================||\n");
                    printf("|| Escolha uma das op��es:                                                ||\n");
                    printf("|| [1] - Iniciar um novo jogo                                             ||\n");
                    printf("|| [2] - Voltar ao menu principal                                         ||\n");
                    printf("||========================================================================||\n");
                    printf("-> \n");
                    scanf("%d",&entFim);

                    if(!(entFim==1||entFim==2)){
                        printf("Op��o inv�lida, tente novamente");
                    }
                LIMPA_TELA;
                }while(!(entFim==1||entFim==2));
                p1.pontos = 0;
                p2.pontos = 0;
            }while(entFim==1);
        }
        else if(entMenu==2){
            LIMPA_TELA;
            printf("Op��o selecionada: [2] - Ranking\n");
            rank = fopen("ranking.txt","r");
            if(rank==NULL){
                printf("Erro em abrir o arquivo");
                exit(1);
            }
            printf("||========================================================================||\n");
            printf("|| ------------------Ranking dos truqueiros de plant�o--------------------||\n");
            printf("||========================================================================||\n");
            while(1){
                fileR = fgets(str,20,rank);
                if(fileR == NULL || feof(rank)){
                    break;
                }
                printf("      %s",str);
            }
            sleep(5);
            LIMPA_TELA;

            entFim = 2;
        }
        else if(entMenu==3){

            LIMPA_TELA;
            printf("Op��o selecionada: [3] - Sobre\n");
            char *arquivo = "banner2.txt";
            FILE *arquivologo = NULL;

            if((arquivologo = fopen(arquivo,"r")) == NULL)
                {
                fprintf(stderr,"erro ao abrir %s\n",arquivo);
                return 1;
                }

            printar_imagem(arquivologo);

            fclose(arquivologo);



            printf("\n||===========================================================================================||\n");
            printf("|| TCD: TRUCO DE TERMINAL                                                                    ||\n");
            printf("||===========================================================================================||\n");
            printf("|| *Este � nosso trabalho de conclus�o da disciplina Programa��o Procedimental, lecionada    ||\n");
            printf("|| pelo professor Claudiney Ramos Tinoco na Universidade Federal de Uberl�ndia.              ||\n");
            printf("|| *Alunos:                                                                                  ||\n");
            printf("||  - Lucas Sabbatini                                                                        ||\n");
            printf("||  - Pedro Simoni                                                                           ||\n");
            printf("||  - Gustavo Amorim                                                                         ||\n");
            printf("||  - Nicolas Moraes                                                                         ||\n");
            printf("|| *Vers�o: 2.1 - Final                                                                      ||\n");
            printf("|| -> Espero que gostem, divirtam-se!!                                                       ||\n");
            printf("||===========================================================================================||\n");
            printf("\n\n\n");
            entFim = 2;
            sleep(10);
            LIMPA_TELA;
        }
        else if(entMenu==4){
            return 0;
        }
    }while(entFim==2);
}



int jogador(int cp1, struct jogador *p1, int maoDeFerro){
    int a;
    printf("\n=======M�o do Jogador======= \n");
        if(maoDeFerro == 1){
        a=jogadorMaodeFerro(cp1, p1);
        }else{

        for(int i = 0; i < 3; i++)
            {
                Carta(p1->cartas[i].valor, p1->cartas[i].naipe, i, p1->cartas[i].jogada, p1->cartas[i].forca);
            }
        do{
        if(cp1==4){ //nesse caso, na l�gica do jogo n�o faz sentido o jogador pedir truco nesse momento
            printf("Digite a carta que voc� ir� jogar: \n->");
        }
        else{
            printf("\nDigite a carta que voc� ir� jogar: \nPara pedir truco: [4] \n->");
        }
        scanf("%d",&a);
        a = a==4?4:a-1;
        if(!(a==0||a==1||a==2||a==4)){
            printf("Op��o inv�lida, digite novamente\n->");
            }
        }while(!(a==0||a==1||a==2||a==4));
    }
    if(a!=4&&p1->cartas[a].jogada == 0){
        printf("Carta repetida, escolha outra carta para ser jogada: ");
        a = jogador(cp1,p1, maoDeFerro);
    }

    sleep(1.5);
    return a;
}



int trucoP1(int *valMao){                             // 1-n�o aceitar 2-aceitar 3-aumentar a aposta //return 0- n�o aceitar return 1- aceitar return 2- aumentar a aposta
    int ent;
    if(*valMao==12){
        printf("A m�o j� est� valendo 12 pontos, n�o h� como aumentar a aposta");
        return 1;
    }
    printf("#############O Bot TRUCOU!#############\nValor atual da m�o: %d - Valor que a m�o passar� a ter: %d\nVai aceitar? [s] ou [n])\n-> ",*valMao,*valMao==1?3:*valMao+3);
    printf("[1]-N�o aceitar\n[2]-Aceitar\n[3]-Aumentar a aposta\n ->");
    do{
        scanf(" %d",&ent);
        if(!(ent==1||ent==2||ent==3)){
            printf("Op��o inv�lida, digite denovo");
        }
        if(ent==1){
            return 0;
        }
        else if(ent==2){
            *valMao = *valMao==1 ? 3:*valMao+3;
            return 1;
        }
        else if(ent==3){
            if(*valMao==12){
                printf("A m�o j� est� valendo 12 pontos, n�o h� como aumentar a aposta");
                return 1;
            }
            *valMao = *valMao==1 ? 3:*valMao+3;
            return 2;
        }
    }while(!(ent==1||ent==2||ent==3));
}

int trucoP2(struct carta cartas[3], int rodada, int *valMao)
{
    int blefe;
    double p;
    rodada += 1;
    srand(time(NULL));
    blefe = rand() % 6;
    p = numero(cartas);


    if(rodada == 1){
        if(p>=10011000000000){
            *valMao = *valMao==1 ? 3:*valMao+3;
            return 2;
        } else if(p>=1200000000){
            *valMao = *valMao==1 ? 3:*valMao+3;
            return 1;
        } else if(blefe == 6) {
            *valMao = *valMao==1 ? 3:*valMao+3;
            return 2;
        } else if(p<1200000000){
            return 0;
        }
    } else if(rodada == 2){
        if(p>=10011000000000){
            *valMao = *valMao==1 ? 3:*valMao+3;
            return 2;
        } else if(p>=1200000000){
            *valMao = *valMao==1 ? 3:*valMao+3;
            return 1;
        } else if(blefe == 6) {
            *valMao = *valMao==1 ? 3:*valMao+3;
            return 2;
        } else {
            return 0;
        }
    } else {
        if(cartas[0].jogada != 0){
            if(cartas[0].forca == 14){
                *valMao = *valMao==1 ? 3:*valMao+3;
                return 2;
            } else if(cartas[0].forca >= 11){
                *valMao = *valMao==1 ? 3:*valMao+3;
                return 1;
            } else if(blefe == 6){
                *valMao = *valMao==1 ? 3:*valMao+3;
                return 2;
            } else {
                return 0;
            }
        } else if(cartas[1].jogada != 0){
            if(cartas[1].forca == 14){
                *valMao = *valMao==1 ? 3:*valMao+3;
                return 2;
            } else if(cartas[1].forca >= 11){
                *valMao = *valMao==1 ? 3:*valMao+3;
                return 1;
            } else if(blefe == 6){
                *valMao = *valMao==1 ? 3:*valMao+3;
                return 2;
            } else {
                return 0;
            }
        } else if(cartas[2].jogada != 0){
            if(cartas[2].forca == 14){
                *valMao = *valMao==1 ? 3:*valMao+3;
                return 2;
            } else if(cartas[2].forca >= 11){
                *valMao = *valMao==1 ? 3:*valMao+3;
                return 1;
            } else if(blefe == 6){
                *valMao = *valMao==1 ? 3:*valMao+3;
                return 2;
            } else {
                return 0;
            }
        }
    }
}

int maoDeOnzeJogador(int *valMao,int *acaba, struct jogador *p1){
    char ent;
    int opt;
    printf("\n==============M�O DE ONZE==================\n");
    printf("\nMeus parabens, voc� atingiu 11 pontos!\n");
    do{
        printf("\n=======M�o do Jogador=======\n");
        for (int i = 0; i < 3; i++){
                Carta(p1->cartas[i].valor, p1->cartas[i].naipe, i, p1->cartas[i].jogada, p1->cartas[i].forca);


        }
        printf("\nVoce vai querer aceitar a m�o de onze? [s] ou [n])\n-> ");
        scanf(" %c",&ent);
        if(!(ent=='s'||ent=='n')){
            printf("Op��o inv�lida, digite novamente.");
        }
        LIMPA_TELA;
    }while(!(ent=='s'||ent=='n'));
    if(ent=='s'){
        *valMao =3;
        return 1;
    }
    else{
        *acaba = 1;
        return 0;
    }
}



int maoDeOnzeBot(int *valMao, int *acaba, struct carta cartas[3]){
    double p;;
    p = numero(cartas);

    if(p>=210000000){
        *valMao =3;
        return 1;
    } else{
        *acaba = 1;
        return 0;
    }

}

void buscarJogador(struct jogador *p1){
    FILE *p;
    char str[50];
    char strNome[20];
    int vit=0,derr=0,procura,achou=0;
    int lenNome;
    int lenStr;
    char *checkErr;
    int ent;
    int i;
    p = fopen("ranking.txt","r");
    if(p==NULL){
        printf("Erro na abertura do arquivo");
        system("pause");
        exit (1);
    }
    do{
        rewind(p);
        if(achou){
            printf("\nN�o conseguimos achar o seu nome, voce quer:\n[1]-Tentar procarar novamente\n[2]-Adicionar um novo nome? \n->");
            scanf("%d",&ent);
            if(ent==1){
                printf("\nDigite o seu nome: ");
            }
        }
        else{
            printf("\nDigite o seu nome: ");
        }
        setbuf(stdin,NULL);
        if(ent==2){
            printf("O seu nome j� foi adicionado!!!");
            break;
        }
        else{
            fgets(strNome,20,stdin);
            strNome[strcspn(strNome, "\r\n")] = '\0';
        }
        lenNome = strlen(strNome);

        do{
            if(feof(p)){
                achou = 1;
                break;
            }

            checkErr = fgets(str,50,p);

            if(checkErr==NULL){
                printf("Erro na leitura do arquivo");
                exit(1);
            }
            lenStr = strlen(str);

            for(i=0;i<lenStr;i++){
                if(str[i]==','){
                    lenStr = i;
                }
            }

            if(check(str,strNome,lenStr,lenNome)){
                procura = 0;
                achou = 0;
                separar(str,&vit,&derr);
            }
            else{
                procura = 1;
            }
        }while(procura);
    }while(achou);
    fclose(p);

    strcpy((*p1).nome,strNome);
    (*p1).vitorias = vit;
    (*p1).derrotas = derr;
}





int check(char *str1,char *str2,int len1,int len2){
    int i,position = 0;
    char copy[50];

    for(i=0;i<50;i++){
        copy[i] = str1[i];
    }
    for(i=0;i<50;i++){
        if(position){
            copy[i] = 0;
        }
        if(copy[i]==','){
            copy[i] = '\0';
            position = i;
        }
    }

    if (len1==len2&&!strcmp(copy,str2)){
        return 1;
    }
    return 0;
}

void separar(char *str,int *vit,int *derr){
    int strLen = strlen(str);
    int len1=0;
    int len2=0;
    int p1=0,p2=0,p3=0;
    int i,j;
    for(i=0;i<strLen;i++){
        if(str[i]==','&&p1){
            p2 = i;
        }
        else if(str[i]==','){
            p1 = i;
        }
    }
    p3 = strLen;
    len1 = p2 - p1 -1;
    len2 = p3 - p2 -1;
    char n1[len1+1];
    char n2[len2+1];

    for(j=0,i=p1+1;i<p2;i++,j++){
        n1[j] = str[i];
    }
    for(j=0,i=p2+1;i<p3;i++,j++){
        n2[j] = str[i];
    }
    n1[len1] = '\0';
    n2[len2] = '\0';

    *vit = atoi(n1);
    *derr = atoi(n2);
}


void escreverJogador(char *strNome,int vit,int derr){
    FILE *p;
    char str[50];
    int procura,achou=0;
    int lenNome;
    int lenStr;
    char *checkErr;
    int i;
    p = fopen("ranking.txt","r+");
    if(p==NULL){
        printf("Erro na abertura do arquivo");
    }



    lenNome = strlen(strNome);
    do{
        if(feof(p)){
            achou = 1;
            break;
        }
        checkErr = fgets(str,50,p);

        if(checkErr==NULL){
            printf("Erro na leitura do arquivo");
            exit(1);
        }
        lenStr = strlen(str);

        for(i=0;i<lenStr;i++){
            if(str[i]==','){
                lenStr = i;
            }
        }

        if(check(str,strNome,lenStr,lenNome)){
            procura = 0;
            achou = 0;
        }
        else{
            procura = 1;
        }
    }while(procura);
    mout(strNome,lenNome,vit,derr,p,achou);
    fclose(p);
}


void mout(char *nome,int lenNome,int vit,int derr,FILE *p,int achou){
    char str[50];
    char vits[10];
    char derrs[10];
    int i,j=0,k=0;
    sprintf(vits, "%d", vit);
    sprintf(derrs, "%d", derr);
    int lenVit=strlen(vits),lenDerr=strlen(derrs);
    int stop = lenNome+lenVit+lenDerr+2;

    for(i=0;i<stop;i++){
        if(i<lenNome){
            str[i] = nome[i];
        }
        else if(i==lenNome){
            str[i] = ',';
        }
        else if(i>lenNome&&i<lenNome+1+lenVit){
            str[i] = vits[j];
            j++;
        }
        else if(i==lenNome+1+lenVit){
            str[i] = ',';
        }
        else{
            str[i] = derrs[k];
            k++;
        }
    }
    str[stop] = '\0';


    if(!achou){
        fseek(p,-(stop+1),SEEK_CUR);
        fprintf(p,"%s",str);
    }
    else{
        fprintf(p,"\n%s",str);
    }

}

void organiza (struct carta *cartas1, struct carta *cartas2, struct carta *cartas3)
{
    struct carta c1, c2, c3;
    c1=*cartas1;
    c2=*cartas2;
    c3=*cartas3;

    if(c1.forca > c2.forca && c1.forca > c3.forca)
    {
        if(c3.forca > c2.forca )
        {
            *cartas2 = c3;
            *cartas3 = c2;
        }
    }

    if(c2.forca > c1.forca && c2.forca > c3.forca)
    {
        if(c1.forca > c3.forca)
        {
            *cartas1 = c2;
            *cartas2 = c1;
            *cartas3 = c3;
        }
        else{
            *cartas1 = c2;
            *cartas2 = c3;
            *cartas3 = c1;
        }
    }
    if(c3.forca > c1.forca && c3.forca > c2.forca)
    {
        if(c1.forca > c2.forca)
        {
            *cartas1 = c3;
            *cartas2 = c1;
            *cartas3 = c2;
        }
        else
        {
            *cartas1 = c3;
            *cartas2 = c2;
            *cartas3 = c1;
        }
    }
}

double numero (struct carta cartas[3])
{

    double p;
    p = 0;

    for(int i=0; i<3; i++){
        switch (cartas[i].forca)
        {
            case 1: p += 1; break;
            case 2: p += (1*10); break;
            case 3: p += (1*100); break;
            case 4: p += (1*1000); break;
            case 5: p += (1*10000); break;
            case 6: p += (1*100000); break;
            case 7: p += (1*1000000); break;
            case 8: p += (1*10000000); break;
            case 9: p += (1*100000000); break;
            case 10: p += (1*1000000000); break;
            case 11: p += (1*10000000000); break;
            case 12: p += (1*100000000000); break;
            case 13: p += (1*1000000000000); break;
            case 14: p += (1*10000000000000); break;
        }
    }

    return p;

}

int combinacao(double p)
{
    if(3<=p && p<=12000000000){
        return 1;
    } else if (100000000002<=p && p<=102000000000){
        return 1;
    } else if (110000000001<=p && p<=111000000000){
        return 2;
    } else if (1000000000002<=p && p<=1001100000000){
        return 1;
    } else if (1010000000001<=p && p<=1010100000000){
        return 3;
    } else if (1011000000000<=p && p<=1100001000000){
        return 2;
    } else if (1100010000000<=p && p<=1110000000000){
        return 3;
    } else if (10000000000002<=p && p<=10000002000000){
        return 1;
    } else if (10000010000001<=p && p<=10001001000000){
        return 3;
    } else if (10001010000000<=p && p<=10002000000000){
        return 2;
    } else if (10010000000001<=p && p<=10110000000000){
        return (3+randomizar(p));
    } else if (11000000000001<=p && p<=11100000000000){
        return randomizar(p);
    }
}

int randomizar(double p){
   srand(time (NULL));
   int truco, rodada;
   double divisor=1000000000000;

   if((p/divisor) == 11){
      truco = (rand()%3 + 1) * 10;
      rodada = rand()%3 + 1;
      return truco+rodada;
   }
   else if((p/divisor) == 10){
      truco = ((rand()%3)+1) * 10;
      return truco;
   }
}

int jogadas(struct carta cartas[3], int PontosBot, int rodada, int PesoCartaPlayer, int PesoRodada, int *JogadasBot, int cangada, int *PrimeiraRodada)
{
    double reCarta;
    int jogada, truco;
    int blefe;
    rodada += 1;
    if(rodada == 1){
        reCarta = numero(cartas);
        jogada = combinacao(reCarta);

        if(jogada >= 10){
            truco = jogada/10;
            jogada = jogada - (truco*10);
        } else if(jogada < 10){
            truco = 100;
        }
    }

    if(truco == rodada){
        return 4;
    }else if (PesoRodada == 2 || PesoRodada == 5 || PesoRodada == 8 || PesoRodada == 11){

    } else if(rodada == 1 && PesoCartaPlayer == 0){
        if(jogada == 1){
            JogadasBot[1] = 2;
            JogadasBot[2] = 1;
            JogadasBot[0] = 0;
        } else if(jogada == 2){
            JogadasBot[1] = 2;
            JogadasBot[2] = 0;
            JogadasBot[0] = 1;
        } else if(jogada == 3){
            JogadasBot[1] = 1;
            JogadasBot[2] = 0;
            JogadasBot[0] = 2;
        }
    } else if(rodada == 1 && PesoCartaPlayer != 0){
        if(cartas[2].forca > PesoCartaPlayer){
            JogadasBot[1] = 1;
            JogadasBot[2] = 0;
            JogadasBot[0] = 2;
        } else if(cartas[1].forca > PesoCartaPlayer){
            JogadasBot[1] = 2;
            JogadasBot[2] = 0;
            JogadasBot[0] = 1;
        } else if(cartas[0].forca > PesoCartaPlayer){
            JogadasBot[1] = 2;
            JogadasBot[2] = 1;
            JogadasBot[0] = 0;
        } else if(cartas[0].forca <= PesoCartaPlayer){
            JogadasBot[0] = 2;
        }
    }else if(rodada == 2 && cangada == 1){
        if(cartas[0].jogada == 0){
            JogadasBot[1] = 1;
            JogadasBot[2] = 2;
        }
        else if(cartas[1].jogada == 0){
            JogadasBot[1] = 0;
            JogadasBot[2] = 2;
        }
        else if(cartas[2].jogada == 0){
            JogadasBot[1] = 0;
            JogadasBot[2] = 1;
        }

    } else if(rodada == 2 && PesoCartaPlayer == 0){
        *PrimeiraRodada = 0;
    } else if(rodada == 2 && PesoCartaPlayer != 0){
        if(cartas[2].forca > PesoCartaPlayer && cartas[2].jogada != 0){
            if(cartas[0].jogada != 0){
                JogadasBot[2] = 0;
            } else if(cartas[1].jogada != 0){
                JogadasBot[2] = 1;
            }
            JogadasBot[1] = 2;
        } else if(cartas[1].forca > PesoCartaPlayer && cartas[1].jogada != 0){
            if(cartas[0].jogada != 0){
                JogadasBot[2] = 0;
            } else if(cartas[2].jogada != 0){
                JogadasBot[2] = 2;
            }
            JogadasBot[1] = 1;
        } else if(cartas[0].forca > PesoCartaPlayer && cartas[0].jogada != 0){
            if(cartas[1].jogada != 0){
                JogadasBot[2] = 1;
            } else if(cartas[2].jogada != 0){
                JogadasBot[2] = 2;
            }
            JogadasBot[1] = 0;
        } else if(cartas[0].forca < PesoCartaPlayer){
            if(cartas[2].jogada == 0){
                JogadasBot[1] = 1;
            } else if(cartas[1].jogada == 0){
                JogadasBot[1] = 2;
            } else if(cartas[0].jogada == 0){
                JogadasBot[1] = 2;
            }
        }
    } else if(rodada == 3 && PesoCartaPlayer == 0 && PesoRodada == 1){
        if(cartas[JogadasBot[2]].forca <= 9 && blefe == 6){
            return 4;
        } else if(cartas[JogadasBot[2]].forca > 10){
            return 4;
        }
    } else if(rodada == 3 && PesoCartaPlayer !=0 && PesoRodada == 1){
        srand(time (NULL));
        blefe = rand()%6;
        if(cartas[JogadasBot[2]].forca > PesoCartaPlayer){
            return 4;
        } else if(cartas[JogadasBot[2]].forca == PesoCartaPlayer && *PrimeiraRodada == 0){
            return 4;
        } else if(cartas[JogadasBot[2]].forca < PesoCartaPlayer && blefe == 6){
            return 4;
        }
    }
    return JogadasBot[rodada-1];
}


//Fun��es Nicolas
void Carta(char valor, char naipe, int op, int jogada, int forca)
{

    if(jogada == 1){


    sleep(1.5);
    printf("==========");
    switch(valor){
        case 'A':
                switch(naipe){
                    case 'O':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|A  |\n");
                        printf("| /\\|\n");
                        printf("|_\\/|\n");
                        printf("\n");
                        printf("�s de Ouro\n");
                        printf("For�a: %d\n", forca);

                        break;
                    case 'E':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|A  |\n");
                        printf("| ^ |\n");
                        printf("|(,)|\n");
                        printf("\n");
                        printf("�s de Espadilha\n");
                        printf("For�a: %d\n", forca);

                        break;
                    case 'C':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|A  |\n");
                        printf("|(`)|\n");
                        printf("|_\\_|\n");
                        printf("\n");
                        printf("�s de Copas\n");
                        printf("For�a: %d\n", forca);
                        break;
                    case 'P':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|A  |\n");
                        printf("| O |\n");
                        printf("|O,O|\n");
                        printf("\n");
                        printf("�s de Paus\n");
                        printf("For�a: %d\n", forca);

                        break;




                }
        break;
        case '2':
                switch(naipe){
                        case 'O':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|2  |\n");
                        printf("| /\\|\n");
                        printf("|_\\/|\n");
                        printf("\n");
                        printf("2 de Ouro\n");
                        printf("For�a: %d\n", forca);

                        break;
                        case 'E':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|2  |\n");
                        printf("| ^ |\n");
                        printf("|(,)|\n");
                        printf("\n");
                        printf("2 de Espadilha\n");
                        printf("For�a: %d\n", forca);


                        break;
                        case 'C':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|2  |\n");
                        printf("|(`)|\n");
                        printf("|_\\_|\n");
                        printf("\n");
                        printf("2 de Copas\n");
                        printf("For�a: %d\n", forca);

                        break;
                        case 'P':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|2  |\n");
                        printf("| O |\n");
                        printf("|O,O|\n");
                        printf("\n");
                        printf("2 de Paus\n");
                        printf("For�a: %d\n", forca);

                        break;
                        }
        break;
        case '3':
                switch(naipe){
                        case 'O':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|3  |\n");
                        printf("| /\\|\n");
                        printf("|_\\/|\n");
                        printf("\n");
                        printf("3 de Ouro\n");
                        printf("For�a: %d\n", forca);

                        break;
                        case 'E':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|3  |\n");
                        printf("| ^ |\n");
                        printf("|(,)|\n");
                        printf("\n");
                        printf("3 de Espadilha\n");
                        printf("For�a: %d\n", forca);

                        break;
                        case 'C':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|3  |\n");
                        printf("|(`)|\n");
                        printf("|_\\_|\n");
                        printf("\n");
                        printf("3 de Copas\n");
                        printf("For�a: %d\n", forca);

                        break;
                        case 'P':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|3  |\n");
                        printf("| O |\n");
                        printf("|O,O|\n");
                        printf("\n");
                        printf("3 de Paus\n");
                        printf("For�a: %d\n", forca);

                        break;
                        }
        break;
        case '4':
                switch(naipe){

                        case 'O':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|4  |\n");
                        printf("| /\\|\n");
                        printf("|_\\/|\n");
                        printf("\n");
                        printf("4 de Ouro\n");
                        printf("For�a: %d\n", forca);

                        break;
                        case 'E':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|4  |\n");
                        printf("| ^ |\n");
                        printf("|(,)|\n");
                        printf("\n");
                        printf("4 de Espadilha\n");
                        printf("For�a: %d\n", forca);

                        break;
                        case 'C':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|4  |\n");
                        printf("|(`)|\n");
                        printf("|_\\_|\n");
                        printf("\n");
                        printf("4 de Copas\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'P':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|4  |\n");
                        printf("| O |\n");
                        printf("|O,O|\n");
                        printf("\n");
                        printf("4 de Paus\n");
                        printf("For�a: %d\n", forca);
                        break;
                        }
        break;
        case '5':
                switch(naipe){
                        case 'O':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|5  |\n");
                        printf("| /\\|\n");
                        printf("|_\\/|\n");
                        printf("\n");
                        printf("5 de Ouro\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'E':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|5  |\n");
                        printf("| ^ |\n");
                        printf("|(,)|\n");
                        printf("\n");
                        printf("5 de Espadilha\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'C':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|5  |\n");
                        printf("|(`)|\n");
                        printf("|_\\_|\n");
                        printf("\n");
                        printf("5 de Copas\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'P':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|5  |\n");
                        printf("| O |\n");
                        printf("|O,O|\n");
                        printf("\n");
                        printf("5 de Paus\n");
                        printf("For�a: %d\n", forca);
                        break;
                        }
        break;
        case '6':
                switch(naipe){
                        case 'O':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|6  |\n");
                        printf("| /\\|\n");
                        printf("|_\\/|\n");
                        printf("\n");
                        printf("6 de Ouro\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'E':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|6  |\n");
                        printf("| ^ |\n");
                        printf("|(,)|\n");
                        printf("\n");
                        printf("6 de Espadilha\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'C':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|6  |\n");
                        printf("|(`)|\n");
                        printf("|_\\_|\n");
                        printf("\n");
                        printf("6 de Copas\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'P':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|6  |\n");
                        printf("| O |\n");
                        printf("|O,O|\n");
                        printf("\n");
                        printf("6 de Paus\n");
                        printf("For�a: %d\n", forca);
                        break;
                        }
        break;
        case '7':
                switch(naipe){
                        case 'O':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|7  |\n");
                        printf("| /\\|\n");
                        printf("|_\\/|\n");
                        printf("\n");
                        printf("7 de Ouro\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'E':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|7  |\n");
                        printf("| ^ |\n");
                        printf("|(,)|\n");
                        printf("\n");
                        printf("7 de Espadilha\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'C':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|7  |\n");
                        printf("|(`)|\n");
                        printf("|_\\_|\n");
                        printf("\n");
                        printf("7 de Copas\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'P':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|7  |\n");
                        printf("| O |\n");
                        printf("|O,O|\n");
                        printf("\n");
                        printf("7 de Paus\n");
                        printf("For�a: %d\n", forca);
                        break;
                        }
        break;
        case 'Q':
                switch(naipe){
                        case 'O':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|Q  |\n");
                        printf("| /\\|\n");
                        printf("|_\\/|\n");
                        printf("\n");
                        printf("Q de Ouro\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'E':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|Q  |\n");
                        printf("| ^ |\n");
                        printf("|(,)|\n");
                        printf("\n");
                        printf("Q de Espadilha\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'C':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|Q  |\n");
                        printf("|(`)|\n");
                        printf("|_\\_|\n");
                        printf("\n");
                        printf("Q de Copas\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'P':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|Q  |\n");
                        printf("| O |\n");
                        printf("|O,O|\n");
                        printf("\n");
                        printf("Q de Paus\n");
                        printf("For�a: %d\n", forca);
                        break;
                        }
        break;
        case 'J':
                switch(naipe){
                        case 'O':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|J  |\n");
                        printf("| /\\|\n");
                        printf("|_\\/|\n");
                        printf("\n");
                        printf("J de Ouro\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'E':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|J  |\n");
                        printf("| ^ |\n");
                        printf("|(,)|\n");
                        printf("\n");
                        printf("J de Espadilha\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'C':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|J  |\n");
                        printf("|(`)|\n");
                        printf("|_\\_|\n");
                        printf("\n");
                        printf("J de Copas\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'P':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|J  |\n");
                        printf("| O |\n");
                        printf("|O,O|\n");
                        printf("\n");
                        printf("J de Paus\n");
                        printf("For�a: %d\n", forca);
                        break;
                        }
        break;
        case 'K':
                switch(naipe){
                        case 'O':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|K  |\n");
                        printf("| /\\|\n");
                        printf("|_\\/|\n");
                        printf("\n");
                        printf("K de Ouro\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'E':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|K  |\n");
                        printf("| ^ |\n");
                        printf("|(,)|\n");
                        printf("\n");
                        printf("K de Espadilha\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'C':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|K  |\n");
                        printf("|(`)|\n");
                        printf("|_\\_|\n");
                        printf("\n");
                        printf("K de Copas\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'P':
                        printf("Carta [%d]\n", op+1);
                        printf(" ___ \n");
                        printf("|K  |\n");
                        printf("| O |\n");
                        printf("|O,O|\n");
                        printf("\n");
                        printf("K de Paus\n");
                        printf("For�a: %d\n", forca);
                        break;
                        }
        break;
}}}

// printar carta da mesa
void cartaMesa(char valor, char naipe, int forca)
{
    sleep(1.5);
    switch(valor){
        case 'A':
                switch(naipe){
                    case 'O':
                        printf(" ___ \n");
                        printf("|A  |\n");
                        printf("| /\\|\n");
                        printf("|_\\/|\n");
                        printf("\n");
                        printf("�s de Ouro\n");
                        printf("For�a: %d\n", forca);

                        break;
                    case 'E':
                        printf(" ___ \n");
                        printf("|A  |\n");
                        printf("| ^ |\n");
                        printf("|(,)|\n");
                        printf("\n");
                        printf("�s de Espadilha\n");
                        printf("For�a: %d\n", forca);

                        break;
                    case 'C':
                        printf(" ___ \n");
                        printf("|A  |\n");
                        printf("|(`)|\n");
                        printf("|_\\_|\n");
                        printf("\n");
                        printf("�s de Copas\n");
                        printf("For�a: %d\n", forca);
                        break;
                    case 'P':
                        printf(" ___ \n");
                        printf("|A  |\n");
                        printf("| O |\n");
                        printf("|O,O|\n");
                        printf("\n");
                        printf("�s de Paus\n");
                        printf("For�a: %d\n", forca);
                        break;




                }
        break;
        case '2':
                switch(naipe){
                        case 'O':
                        printf(" ___ \n");
                        printf("|2  |\n");
                        printf("| /\\|\n");
                        printf("|_\\/|\n");
                        printf("\n");
                        printf("2 de Ouro\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'E':
                        printf(" ___ \n");
                        printf("|2  |\n");
                        printf("| ^ |\n");
                        printf("|(,)|\n");
                        printf("\n");
                        printf("2 de Espadilha\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'C':
                        printf(" ___ \n");
                        printf("|2  |\n");
                        printf("|(`)|\n");
                        printf("|_\\_|\n");
                        printf("\n");
                        printf("2 de Copas\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'P':
                        printf(" ___ \n");
                        printf("|2  |\n");
                        printf("| O |\n");
                        printf("|O,O|\n");
                        printf("\n");
                        printf("2 de Paus\n");
                        printf("For�a: %d\n", forca);
                        break;
                        }
        break;
        case '3':
                switch(naipe){
                        case 'O':
                        printf(" ___ \n");
                        printf("|3  |\n");
                        printf("| /\\|\n");
                        printf("|_\\/|\n");
                        printf("\n");
                        printf("3 de Ouro\n");
                        printf("For�a: %d\n", forca);

                        break;
                        case 'E':
                        printf(" ___ \n");
                        printf("|3  |\n");
                        printf("| ^ |\n");
                        printf("|(,)|\n");
                        printf("\n");
                        printf("3 de Espadilha\n");
                        printf("For�a: %d\n", forca);

                        break;
                        case 'C':
                        printf(" ___ \n");
                        printf("|3  |\n");
                        printf("|(`)|\n");
                        printf("|_\\_|\n");
                        printf("\n");
                        printf("3 de Copas\n");
                        printf("For�a: %d\n", forca);

                        break;
                        case 'P':
                        printf(" ___ \n");
                        printf("|3  |\n");
                        printf("| O |\n");
                        printf("|O,O|\n");
                        printf("\n");
                        printf("3 de Paus\n");
                        printf("For�a: %d\n", forca);

                        break;
                        }
        break;
        case '4':
                switch(naipe){

                        case 'O':
                        printf(" ___ \n");
                        printf("|4  |\n");
                        printf("| /\\|\n");
                        printf("|_\\/|\n");
                        printf("\n");
                        printf("4 de Ouro\n");
                        printf("For�a: %d\n", forca);

                        break;
                        case 'E':
                        printf(" ___ \n");
                        printf("|4  |\n");
                        printf("| ^ |\n");
                        printf("|(,)|\n");
                        printf("\n");
                        printf("4 de Espadilha\n");
                        printf("For�a: %d\n", forca);

                        break;
                        case 'C':
                        printf(" ___ \n");
                        printf("|4  |\n");
                        printf("|(`)|\n");
                        printf("|_\\_|\n");
                        printf("\n");
                        printf("4 de Copas\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'P':
                        printf(" ___ \n");
                        printf("|4  |\n");
                        printf("| O |\n");
                        printf("|O,O|\n");
                        printf("\n");
                        printf("4 de Paus\n");
                        printf("For�a: %d\n", forca);
                        break;
                        }
        break;
        case '5':
                switch(naipe){
                        case 'O':
                        printf(" ___ \n");
                        printf("|5  |\n");
                        printf("| /\\|\n");
                        printf("|_\\/|\n");
                        printf("\n");
                        printf("5 de Ouro\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'E':
                        printf(" ___ \n");
                        printf("|5  |\n");
                        printf("| ^ |\n");
                        printf("|(,)|\n");
                        printf("\n");
                        printf("5 de Espadilha\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'C':
                        printf(" ___ \n");
                        printf("|5  |\n");
                        printf("|(`)|\n");
                        printf("|_\\_|\n");
                        printf("\n");
                        printf("5 de Copas\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'P':
                        printf(" ___ \n");
                        printf("|5  |\n");
                        printf("| O |\n");
                        printf("|O,O|\n");
                        printf("\n");
                        printf("5 de Paus\n");
                        printf("For�a: %d\n", forca);
                        break;
                        }
        break;
        case '6':
                switch(naipe){
                        case 'O':
                        printf(" ___ \n");
                        printf("|6  |\n");
                        printf("| /\\|\n");
                        printf("|_\\/|\n");
                        printf("\n");
                        printf("6 de Ouro\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'E':
                        printf(" ___ \n");
                        printf("|6  |\n");
                        printf("| ^ |\n");
                        printf("|(,)|\n");
                        printf("\n");
                        printf("6 de Espadilha\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'C':
                        printf(" ___ \n");
                        printf("|6  |\n");
                        printf("|(`)|\n");
                        printf("|_\\_|\n");
                        printf("\n");
                        printf("6 de Copas\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'P':
                        printf(" ___ \n");
                        printf("|6  |\n");
                        printf("| O |\n");
                        printf("|O,O|\n");
                        printf("\n");
                        printf("6 de Paus\n");
                        printf("For�a: %d\n", forca);
                        break;
                        }
        break;
        case '7':
                switch(naipe){
                        case 'O':
                        printf(" ___ \n");
                        printf("|7  |\n");
                        printf("| /\\|\n");
                        printf("|_\\/|\n");
                        printf("\n");
                        printf("7 de Ouro\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'E':
                        printf(" ___ \n");
                        printf("|7  |\n");
                        printf("| ^ |\n");
                        printf("|(,)|\n");
                        printf("\n");
                        printf("7 de Espadilha\n");
                        printf("For�a: %d\n", forca);

                        break;
                        case 'C':

                        printf(" ___ \n");
                        printf("|7  |\n");
                        printf("|(`)|\n");
                        printf("|_\\_|\n");
                        printf("\n");
                        printf("7 de Copas\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'P':
                        printf(" ___ \n");
                        printf("|7  |\n");
                        printf("| O |\n");
                        printf("|O,O|\n");
                        printf("\n");
                        printf("7 de Paus\n");
                        printf("For�a: %d\n", forca);
                        break;
                        }
        break;
        case 'Q':
                switch(naipe){
                        case 'O':

                        printf(" ___ \n");
                        printf("|Q  |\n");
                        printf("| /\\|\n");
                        printf("|_\\/|\n");
                        printf("\n");
                        printf("Q de Ouro\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'E':

                        printf(" ___ \n");
                        printf("|Q  |\n");
                        printf("| ^ |\n");
                        printf("|(,)|\n");
                        printf("\n");
                        printf("Q de Espadilha\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'C':

                        printf(" ___ \n");
                        printf("|Q  |\n");
                        printf("|(`)|\n");
                        printf("|_\\_|\n");
                        printf("\n");
                        printf("Q de Copas\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'P':

                        printf(" ___ \n");
                        printf("|Q  |\n");
                        printf("| O |\n");
                        printf("|O,O|\n");
                        printf("\n");
                        printf("Q de Paus\n");
                        printf("For�a: %d\n", forca);
                        break;
                        }
        break;
        case 'J':
                switch(naipe){
                        case 'O':

                        printf(" ___ \n");
                        printf("|J  |\n");
                        printf("| /\\|\n");
                        printf("|_\\/|\n");
                        printf("\n");
                        printf("J de Ouro\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'E':

                        printf(" ___ \n");
                        printf("|J  |\n");
                        printf("| ^ |\n");
                        printf("|(,)|\n");
                        printf("\n");
                        printf("J de Espadilha\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'C':

                        printf(" ___ \n");
                        printf("|J  |\n");
                        printf("|(`)|\n");
                        printf("|_\\_|\n");
                        printf("\n");
                        printf("J de Copas\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'P':

                        printf(" ___ \n");
                        printf("|J  |\n");
                        printf("| O |\n");
                        printf("|O,O|\n");
                        printf("\n");
                        printf("J de Paus\n");
                        printf("For�a: %d\n", forca);
                        break;
                        }
        break;
        case 'K':
                switch(naipe){
                        case 'O':

                        printf(" ___ \n");
                        printf("|K  |\n");
                        printf("| /\\|\n");
                        printf("|_\\/|\n");
                        printf("\n");
                        printf("K de Ouro\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'E':

                        printf(" ___ \n");
                        printf("|K  |\n");
                        printf("| ^ |\n");
                        printf("|(,)|\n");
                        printf("\n");
                        printf("K de Espadilha\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'C':

                        printf(" ___ \n");
                        printf("|K  |\n");
                        printf("|(`)|\n");
                        printf("|_\\_|\n");
                        printf("\n");
                        printf("K de Copas\n");
                        printf("For�a: %d\n", forca);
                        break;
                        case 'P':

                        printf(" ___ \n");
                        printf("|K  |\n");
                        printf("| O |\n");
                        printf("|O,O|\n");
                        printf("\n");
                        printf("K de Paus\n");
                        printf("For�a: %d\n", forca);
                        break;
                        }
        break;
}}


void printar_imagem(FILE *fptr)
{
    char leitura[MAX];

    while(fgets(leitura,sizeof(leitura),fptr) != NULL)
        printf("%s",leitura);
}
void printar_logo(FILE *arquivologo)
{
    char tamanho[MAX];
    while(fgets(tamanho, sizeof(tamanho), arquivologo) != NULL)
        printf("%s", tamanho);
}


int jogadorMaodeFerro(int cp1, struct jogador *p1){
    int a;
    for(int i = 0; i < 3; i++)
    {
        carta_virada(i, p1->cartas[i].jogada);
        sleep(1);
    }
    printf("Informa�ao: Na m�o de ferro n�o � poss�vel pedir truco!\n");
        printf("Digite a carta que voc� ir� jogar: \n->");
    scanf("%d",&a);
    a = a==4?4:a-1;
    if(a!=4&&p1->cartas[a].jogada == 0){
        printf("\nCarta j� jogada, escolha outra: ");
        a = jogadorMaodeFerro(cp1,p1);

    }
    printf(" a: %d\n", a);
    printf("Carta escolhida: [%d]\n", a+1);
    Carta(p1->cartas[a].valor, p1->cartas[a].naipe, a, p1->cartas[a].jogada, p1->cartas[a].forca);
    sleep(2.5);


    return a;
}
int carta_virada(int op, int jogada)
{
    if(jogada == 1){
    printf("Atributo Jogada: %d\n", jogada);
    printf("Carta [%d]\n", op+1);
    printf(" ___\n|\\ /|\n|[=]|\n|/_\\|");
    printf("\n? de ????\n");
    printf("For�a: ??\n\n");
    return 0;
}
}


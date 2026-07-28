
#include <stdio.h> // biblioteca necessaria para permitir o uso de algumas funcoes essenciais 
#include <windows.h> // biblioteca necessaria para pegar informacoes vindas via USB
#include <stdlib.h> // biblioteca necessaria para permitir o uso de algumas funcoes essenciais 
#include <math.h> // incluimos essa biblioteca para permitir contas matematicas
#include <time.h>  // biblioteca que permite ao programa ler o relógio do computador

#ifndef M_PI
    #define M_PI 3.14159265358979323846 // definimos o valor de pi 
#endif
#define RAIO_TERRA_METROS 6371000.0 // definimos o valor do raio da terra

double lat_inicial = 0, lon_inicial = 0; // criamos e inicializamos as viariaveis para latitude e longitude iniciais 
double lat_atual = 0, lon_atual = 0; // criamos e inicializamos as viariaveis para latitude e longitude atuais
float distancia_lixeira = 0; // criamos e inicializamos a variavel para a distancia da lixeira
int seguranca_ativada = 0; // criamos e inicializamos essa variavel para sabermos sobre a seguranca da lixeira

int checar_movimentacao(double lat_b, double lon_b, double lat_a, double lon_a, double limite); // chama a funcao que calcula se o sistema saiu do lugar comparando a posicao antiga com a nova
void capturar_dados_arduino(); // chama a funcao para pegar os dados enviados pelo arduino

void registrar_no_arquivo(float dist, float port, double lat, double lon) { // Cria a funcao que grava a distancia, porta e GPS no arquivo
    FILE *arq = fopen("historico_coletas.csv", "a");  // Abre a planilha "historico_coletas" para salvar novos dados no final dela
    if (arq == NULL) return; // caso nao abra a planilha, ele desiste para nao travar o programa

    fseek(arq, 0, SEEK_END); // escreve o ultimo dado na ultima linha
    if (ftell(arq) == 0) { // se a planilha estiver vazia
        fprintf(arq, "Data;Hora;Distancia;Porcentagem;Latitude;Longitude\n"); // ele printa isso na planilha
    }

    time_t t = time(NULL); // Pega a hora e a data atuais do relógio do computador
    struct tm tm = *localtime(&t); // Organiza essa informação em formato de ano, mês, dia e hora

    char s_dist[20], s_port[20], s_lat[20], s_lon[20]; // buffers para guardar textos

    // 1. Escreve os números com ponto nos buffers de texto
    sprintf(s_dist, "%.2f", dist);
    sprintf(s_port, "%.2f", port);
    sprintf(s_lat, "%.5f", lat);
    sprintf(s_lon, "%.5f", lon);

    // 2. varre cada texto e troca o '.' por ','
    char *p;
    if ((p = strchr(s_dist, '.'))) *p = ',';
    if ((p = strchr(s_port, '.'))) *p = ',';
    if ((p = strchr(s_lat, '.')))  *p = ',';
    if ((p = strchr(s_lon, '.')))  *p = ',';

    // 3. Grava no arquivo os textos já com a vírgula padrão do Excel BR
    fprintf(arq, "%02d/%02d/%04d;%02d:%02d:%02d;%s;%s;%s;%s\n",
            tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
            tm.tm_hour, tm.tm_min, tm.tm_sec,
            s_dist, s_port, s_lat, s_lon);

    fclose(arq); // fecha a planilha e salva os dados
}

void abrir_no_excel() { // criamos a funcao para abrir oarquivo do excel 
    printf("\n[SISTEMA] Abrindo o historico no Excel...\n"); // printa uma mensagem de confirmacao
    ShellExecute(NULL, "open", "C:\\Users\\lucas\\OneDrive\\Desktop\\UPX1 - Lixeira Inteligente\\grafico_de_dados.xlsx", NULL, NULL, SW_SHOWNORMAL); // abrindo o arquivo
}



int main() {
    int menu = 0;

    while (menu != 4) { // o menu fica reaparecendo ate o usuario nao apertar o numero 4 
        printf("\n-----------------------------SISTEMA DE MONITORAMENTO-----------------------------\n");
        printf("[1] - Ver estado da lixeira\n"); // interface
        printf("[2] - Verificar Seguranca\n"); // interface
        printf("[3] - Abrir Relatorio no Excel\n"); // interface
		printf("[4] - Sair\n"); // interface
        printf("Escolha: "); // interface
        scanf("%i", &menu); // selecione uma opcao 

        if (menu == 1 || menu == 2) {
            capturar_dados_arduino(); // chama a funcao e coleta dados do arduino 
        }

        if (menu == 1) {
            printf("\n-----------------------------------STATUS ATUAL-----------------------------------\n\n");
            if (distancia_lixeira <5) printf("Lixeira: Cheia | "); // funcionamneto da lixeira
            else if (distancia_lixeira < 25) printf("Lixeira: Media | "); // funcionamneto da lixeira
            else printf("Lixeira: Vazia | "); // funcionamneto da lixeira
            float porcentagem = ((34-distancia_lixeira)/34)*100; // transformamos a distancia em porcentagem 
            printf("%.2f%% | ", porcentagem); // mostra a porcentagem 
            for(int i = 0; i < 20; i++){  // transformamos a porcentagem em representacao grafica
            	if(i <= porcentagem/5){ // cada hashtag vale 5%
            		printf("#");
				}
				else{
					printf("-");
				}
			}
			printf(" |\n");
            
            printf("\nDistancia: %.2f cm | ", distancia_lixeira); // mostra a distancia 
            printf("Localizacao: Lat %.5f | Lon %.5f\n", lat_atual, lon_atual); // mostra as coordenadas
            
            registrar_no_arquivo(distancia_lixeira, porcentagem, lat_atual, lon_atual); // coloca todas essas informacoes no arquivo 
    		
            if (seguranca_ativada == 0) { 
                lat_inicial = lat_atual;  // define as posicoes de seguranca
                lon_inicial = lon_atual; // define as posicoes de seguranca
                seguranca_ativada = 1; // define a seguranca
                printf("\n[SEGURANCA] Localizacao inicial definida: %.5f, %.5f\n", lat_inicial, lon_inicial);
                printf("Monitoramento ativado!\n");
        	}
        } 
        else if (menu == 2) {
            if (seguranca_ativada == 0) {
                lat_inicial = lat_atual; // define as posicoes de seguranca
                lon_inicial = lon_atual; // define as posicoes de seguranca
                seguranca_ativada = 1; // define a seguranca
                printf("\n[SEGURANCA] Localizacao inicial definida: %.5f, %.5f\n", lat_inicial, lon_inicial);
                printf("Monitoramento ativado!\n");
            } else {
                printf("\n-----------------------------VERIFICACAO DE SEGURANCA-----------------------------\n\n");
                printf("Origem: %.5f, %.5f | Atual: %.5f, %.5f\n\n", lat_inicial, lon_inicial, lat_atual, lon_atual);
                
                if (checar_movimentacao(lat_inicial, lon_inicial, lat_atual, lon_atual, 100.0)) { // chama a funcao para comparar as coordenadas e verificar seguranca
                    printf("ALERTA: A lixeira foi movida mais de 100 metros\n");
                } else { 
                    printf("STATUS: Segura (Dentro do raio de 100m)\n");
                }
            }
        }
        if (menu == 3) {
   			 abrir_no_excel(); // chama a funcao para abrir o arquivo excel
		}
    }
    return 0;
}

void capturar_dados_arduino() { // criamos uma funcao para pegar os dados do arduino
    HANDLE porta; // cria o "canal" de conexao para o computador conseguir ler o cabo USB
    DCB config = {0}; // prepara uma lista de configuracao do cabo USB totalmente limpa
    COMMTIMEOUTS timeout = {0}; // cria um limite de tempo para o programa nao travar se o radio sumir
    porta = CreateFile("COM5", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL); // Conecta o programa na entrada USB (COM5 -> numero da porta) do computador

    if (porta == INVALID_HANDLE_VALUE) { // se ele nao conseguir se comunicar com a entrada USB
        printf("Erro: Arduino nao encontrado na COM5.\n"); // printa uma mensagem de rro 
        return;
    }

	// esse bloco resumidamente configura a velocidade e as regras de comunicação da porta USB para combinar com o lora
    config.DCBlength = sizeof(config); //  mede o tamanho da lista para o windonws saber quanto ela ocupa
    GetCommState(porta, &config); // pega as configuracoes que a porta USB ja tem no momento
    config.BaudRate = CBR_9600; // define a taxa de transmissao de informacao em 9600 bits por segundo
    config.ByteSize = 8; // cada pacote de texto tera o tamanho de 8 bits 
    config.StopBits = ONESTOPBIT; // havera um sinal de parada para cada mensagem 
    config.Parity = NOPARITY; // Desliga a verificação de erros no cabo para ter mais velocidade
    SetCommState(porta, &config); // Salva e aplica todas essas regras na porta USB do computador

    timeout.ReadIntervalTimeout = 50;  // Define que o computador espera no máximo 50 milissegundos entre uma letra e outra
    timeout.ReadTotalTimeoutConstant = 50; // Adiciona um bônus fixo de 50 milissegundos no tempo total de espera pela resposta
    SetCommTimeouts(porta, &timeout); // Ativa e aplica todas as regras de tempo que configuramos na porta USB

    char buffer[50]; // cria uma string
    char c; 
    DWORD bytes; // Cria uma variavel para contar quantas letras o computador leu da porta USB
    int i = 0, etapa = 0;

    // Loop para pegar 1 leitura completa (3 valores)
    for (int j = 0; j < 3;) {
        if (ReadFile(porta, &c, 1, &bytes, NULL) && bytes > 0) { // Tenta ler 1 letra da porta USB e confere se realmente chegou algo
            if (c == '\n') {  // Se o caractere for um enter
                buffer[i] = '\0'; // Coloca o ponto final no texto
                float valor = atof(buffer); // Transforma o texto guardado no buffer em um número decimal
                if (etapa == 0) { distancia_lixeira = valor; etapa = 1; } // Guarda a distância da lixeira e passa para a próxima etapa (coleta do GPS)
                else if (etapa == 1) { lat_atual = valor; etapa = 2; } // Guarda a latitude atual e avança para a etapa da longitude
                else if (etapa == 2) { lon_atual = valor; etapa = 0; j++; } // Guarda a longitude atual
                i = 0;
            } else if (c != '\r') { // Se a letra não for o "Enter", continua juntando o texto
                buffer[i++] = c; // Guarda a letra atual na lista e pula para a próxima posição
            }
        }
    }
    CloseHandle(porta); // fecha a comunicacao com a porta
}

int checar_movimentacao(double lat_b, double lon_b, double lat_a, double lon_a, double limite) { // criamos uma funcao para comparar a coordenada atual com a antiga
    double dLat = (lat_a - lat_b) * M_PI / 180.0; // contas para saber a diferenca da distancia e saber se foi roubado ou nao 
    double dLon = (lon_a - lon_b) * M_PI / 180.0; // contas para saber a diferenca da distancia e saber se foi roubado ou nao 
    double a = sin(dLat / 2) * sin(dLat / 2) + // contas para saber a diferenca da distancia e saber se foi roubado ou nao 
               cos(lat_b * M_PI / 180.0) * cos(lat_a * M_PI / 180.0) * // contas para saber a diferenca da distancia e saber se foi roubado ou nao 
               sin(dLon / 2) * sin(dLon / 2);  // contas para saber a diferenca da distancia e saber se foi roubado ou nao 
    double c = 2 * atan2(sqrt(a), sqrt(1 - a)); // contas para saber a diferenca da distancia e saber se foi roubado ou nao 
    return (RAIO_TERRA_METROS * c >= limite); // alerta de roubo 
}

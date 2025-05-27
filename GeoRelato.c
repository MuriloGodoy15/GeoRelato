#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_RELATORES 100
#define MAX_RELATOS 1000
#define PI 3.141592653589793
#define RAIO_TERRA_KM 6371.0
#define RAIO_MAXIMO 10.0

typedef struct {
    char nome[100];
    char documento[20];
    char email[100];
    char telefone[20];
    double latitude;
    double longitude;
} Relator;

typedef struct {
    char tipo[50];
    char descricao[300];
    char data[11];  // "DD/MM/AAAA"
    char hora[6];   // "HH:MM"
    double latitude;
    double longitude;
} Relato;

Relator relatores[MAX_RELATORES];
Relato relatos[MAX_RELATOS];
int total_relat = 0;
int total_relato = 0;

double centro_latitude = -23.5505;
double centro_longitude = -46.6333;

double to_rad(double degree) {
    return degree * (PI / 180.0);
}

double calcular_distancia(double lat1, double lon1, double lat2, double lon2) {
    lat1 = to_rad(lat1);
    lon1 = to_rad(lon1);
    lat2 = to_rad(lat2);
    lon2 = to_rad(lon2);

    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;

    double a = pow(sin(dlat / 2), 2) +
               cos(lat1) * cos(lat2) *
               pow(sin(dlon / 2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return RAIO_TERRA_KM * c;
}

int data_para_inteiro(const char *data) {
    int d, m, a;
    sscanf(data, "%d/%d/%d", &d, &m, &a);
    return a * 10000 + m * 100 + d;
}

void to_lower(char *str) {
    for (; *str; str++) *str = tolower(*str);
}

void cadastrar_relato_automatically(char *tipo, char *desc, char *data, char *hora, double lat, double lon) {
    if (total_relato >= MAX_RELATOS) return;

    double distancia = calcular_distancia(centro_latitude, centro_longitude, lat, lon);
    if (distancia > RAIO_MAXIMO) return;

    Relato r;
    strcpy(r.tipo, tipo);
    strcpy(r.descricao, desc);
    strcpy(r.data, data);
    strcpy(r.hora, hora);
    r.latitude = lat;
    r.longitude = lon;

    relatos[total_relato++] = r;
}

void buscar_por_tipo() {
    char tipo[50];
    printf("Digite o tipo de catástrofe: ");
    scanf(" %[^\n]", tipo);
    to_lower(tipo);

    int encontrados = 0;
    for (int i = 0; i < total_relato; i++) {
        char tipo_relato[50];
        strcpy(tipo_relato, relatos[i].tipo);
        to_lower(tipo_relato);

        if (strcmp(tipo_relato, tipo) == 0) {
            printf("\n[%d] %s - %s\nDescrição: %s\nCoordenadas: %.4f, %.4f\n",
                   i + 1, relatos[i].data, relatos[i].hora,
                   relatos[i].descricao, relatos[i].latitude, relatos[i].longitude);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("Nenhum relato encontrado para o tipo '%s'.\n", tipo);
    }
}

void buscar_por_periodo() {
    char inicio[11], fim[11];
    printf("Data início (DD/MM/AAAA): ");
    scanf(" %[^\n]", inicio);
    printf("Data fim (DD/MM/AAAA): ");
    scanf(" %[^\n]", fim);

    int data_ini = data_para_inteiro(inicio);
    int data_fim = data_para_inteiro(fim);

    int encontrados = 0;
    for (int i = 0; i < total_relato; i++) {
        int data_relato = data_para_inteiro(relatos[i].data);
        if (data_relato >= data_ini && data_relato <= data_fim) {
            printf("\n[%d] %s - %s\nTipo: %s\nDescrição: %s\nCoordenadas: %.4f, %.4f\n",
                   i + 1, relatos[i].data, relatos[i].hora, relatos[i].tipo,
                   relatos[i].descricao, relatos[i].latitude, relatos[i].longitude);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("Nenhum relato encontrado no período informado.\n");
    }
}

void menu() {
    int opcao;
    do {
        printf("\n--- GEORELATO ---\n");
        printf("1. Cadastrar relator\n");
        printf("2. Cadastrar relato\n");
        printf("3. Buscar relatos por tipo\n");
        printf("4. Buscar relatos por período\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: cadastrar_relator(); break;
            case 2: cadastrar_relato(); break;
            case 3: buscar_por_tipo(); break;
            case 4: buscar_por_periodo(); break;
            case 0: printf("Encerrando...\n"); break;
            default: printf("Opção inválida.\n");
        }

    } while (opcao != 0);
}

int main() {
    // Relatos automáticos
    cadastrar_relato_automatically("Inundação", "Rua alagada no centro da cidade.", "20/05/2025", "14:30", -23.5520, -46.6340);
    cadastrar_relato_automatically("Deslizamento", "Barranco caiu próximo à escola.", "21/05/2025", "08:15", -23.5515, -46.6339);
    cadastrar_relato_automatically("Incêndio", "Incêndio em depósito de madeira.", "22/05/2025", "17:45", -23.5508, -46.6322);

    menu();
    return 0;
}

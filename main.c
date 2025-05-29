#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_RELATORES 100
#define MAX_RELATOS 1000
#define PI 3.141592653589793
#define RAIO_TERRA_KM 6371.0
#define RAIO_MAXIMO 10.0

// -------------------- STRUCTS --------------------

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

// -------------------- VARIÁVEIS GLOBAIS --------------------

Relator relatores[MAX_RELATORES];
Relato relatos[MAX_RELATOS];
int total_relat = 0;
int total_relato = 0;

// Localização central
double centro_latitude = -23.5505;  // São Paulo
double centro_longitude = -46.6333;

// -------------------- PROTÓTIPOS --------------------

double calcular_distancia(double lat1, double lon1, double lat2, double lon2);
void cadastrar_relator();
void cadastrar_relato();
void buscar_por_tipo();
void buscar_por_periodo();
void relatos_exemplo();
void menu();

// -------------------- FUNÇÕES AUXILIARES --------------------

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

// -------------------- FUNÇÕES PRINCIPAIS --------------------

void cadastrar_relator() {
    if (total_relat >= MAX_RELATORES) {
        printf("Limite de relatores atingido.\n");
        return;
    }

    Relator r;
    printf("Nome completo: ");
    scanf(" %[^\n]", r.nome);
    printf("Documento: ");
    scanf(" %[^\n]", r.documento);
    printf("Email: ");
    scanf(" %[^\n]", r.email);
    printf("Telefone: ");
    scanf(" %[^\n]", r.telefone);
    printf("Latitude: ");
    scanf("%lf", &r.latitude);
    printf("Longitude: ");
    scanf("%lf", &r.longitude);

    relatores[total_relat++] = r;
    printf("Relator cadastrado com sucesso.\n");
}

void cadastrar_relato() {
    if (total_relato >= MAX_RELATOS) {
        printf("Limite de relatos atingido.\n");
        return;
    }

    Relato r;
    printf("Tipo de catástrofe: ");
    scanf(" %[^\n]", r.tipo);
    printf("Descrição: ");
    scanf(" %[^\n]", r.descricao);
    printf("Data (DD/MM/AAAA): ");
    scanf(" %[^\n]", r.data);
    printf("Hora (HH:MM): ");
    scanf(" %[^\n]", r.hora);
    printf("Latitude: ");
    scanf("%lf", &r.latitude);
    printf("Longitude: ");
    scanf("%lf", &r.longitude);

    double distancia = calcular_distancia(centro_latitude, centro_longitude, r.latitude, r.longitude);
    if (distancia > RAIO_MAXIMO) {
        printf("Erro: local do relato está fora do raio de 10 km (%.2f km).\n", distancia);
        return;
    }

    relatos[total_relato++] = r;
    printf("Relato cadastrado com sucesso (distância: %.2f km).\n", distancia);
}

void buscar_por_tipo() {
    char tipo[50];
    printf("Digite o tipo de catástrofe: ");
    scanf(" %[^\n]", tipo);

    int encontrados = 0;
    for (int i = 0; i < total_relato; i++) {
        if (strcmp(relatos[i].tipo, tipo) == 0) {
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

    int encontrados = 0;
    for (int i = 0; i < total_relato; i++) {
        if (strcmp(relatos[i].data, inicio) >= 0 && strcmp(relatos[i].data, fim) <= 0) {
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

void relatos_exemplo() {
    Relato r1 = {"Enchente", "Rua completamente alagada", "25/05/2025", "14:00", -23.5510, -46.6340};
    Relato r2 = {"Deslizamento", "Barranco cedeu próximo à escola", "24/05/2025", "10:30", -23.5520, -46.6350};
    Relato r3 = {"Incêndio", "Incêndio em residência", "23/05/2025", "08:15", -23.5530, -46.6360};

    relatos[total_relato++] = r1;
    relatos[total_relato++] = r2;
    relatos[total_relato++] = r3;
}

// -------------------- MENU PRINCIPAL --------------------

void menu() {
    int opcao;
    do {
        printf("\n--- GEORELATO ---\n");
        printf("o ponto de referência é SP (Latitude: -23.5505 e Longitude: -46.6333)\n");
        printf("1. Cadastrar relator\n");
        printf("2. Cadastrar relato\n");
        printf("3. Buscar relatos por tipo\n");
        printf("4. Buscar relatos por período\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Consumir o \n

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

// -------------------- MAIN --------------------

int main() {
    relatos_exemplo();
    menu();
    return 0;
}
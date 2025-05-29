#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_RELATOS 100
#define MAX_RELATORES 50
#define RAIO_MAXIMO 10.0

// Estrutura para relato
typedef struct {
    char tipo[50];
    char descricao[200];
    char data[11];
    char hora[6];
    double latitude;
    double longitude;
} Relato;

// Estrutura para relator (com relatos aninhados)
typedef struct {
    char nome[100];
    char documento[20];
    char email[100];
    char telefone[20];
    double latitude;
    double longitude;
    Relato relatos[MAX_RELATOS];
    int total_relatos;
} Relator;

Relator relatores[MAX_RELATORES];
int total_relat = 0;

// Coordenadas do centro de referência
double centro_latitude = -23.5505;
double centro_longitude = -46.6333;

double calcular_distancia(double lat1, double lon1, double lat2, double lon2) {
    double pi = 3.141592653589793;
    double R = 6371.0; // raio da Terra em km
    double dLat = (lat2 - lat1) * pi / 180.0;
    double dLon = (lon2 - lon1) * pi / 180.0;

    lat1 = lat1 * pi / 180.0;
    lat2 = lat2 * pi / 180.0;

    double a = sin(dLat/2) * sin(dLat/2) + sin(dLon/2) * sin(dLon/2) * cos(lat1) * cos(lat2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));

    return R * c;
}

void cadastrar_relator() {
    if (total_relat >= MAX_RELATORES) {
        printf("Limite de relatores atingido.\n");
        return;
    }

    Relator r;
    printf("Nome: ");
    scanf(" %[^"]", r.nome);
    printf("Documento: ");
    scanf(" %[^"]", r.documento);
    printf("Email: ");
    scanf(" %[^"]", r.email);
    printf("Telefone: ");
    scanf(" %[^"]", r.telefone);
    printf("Latitude: ");
    scanf("%lf", &r.latitude);
    printf("Longitude: ");
    scanf("%lf", &r.longitude);
    r.total_relatos = 0;

    relatores[total_relat++] = r;
    printf("Relator cadastrado com sucesso.\n");
}

void cadastrar_relato() {
    if (total_relat == 0) {
        printf("Nenhum relator cadastrado. Cadastre um relator primeiro.\n");
        return;
    }

    printf("Selecione o relator pelo número:\n");
    for (int i = 0; i < total_relat; i++) {
        printf("[%d] %s\n", i + 1, relatores[i].nome);
    }

    int indice;
    scanf("%d", &indice);
    indice--;

    if (indice < 0 || indice >= total_relat) {
        printf("Relator inválido.\n");
        return;
    }

    if (relatores[indice].total_relatos >= MAX_RELATOS) {
        printf("Limite de relatos atingido para este relator.\n");
        return;
    }

    Relato r;
    printf("Tipo de catástrofe: ");
    scanf(" %[^"]", r.tipo);
    printf("Descrição: ");
    scanf(" %[^"]", r.descricao);
    printf("Data (DD/MM/AAAA): ");
    scanf(" %[^"]", r.data);
    printf("Hora (HH:MM): ");
    scanf(" %[^"]", r.hora);
    printf("Latitude: ");
    scanf("%lf", &r.latitude);
    printf("Longitude: ");
    scanf("%lf", &r.longitude);

    double distancia = calcular_distancia(centro_latitude, centro_longitude, r.latitude, r.longitude);
    if (distancia > RAIO_MAXIMO) {
        printf("Erro: local do relato está fora do raio de 10 km (%.2f km).\n", distancia);
        return;
    }

    relatores[indice].relatos[relatores[indice].total_relatos++] = r;
    printf("Relato cadastrado com sucesso (distância: %.2f km).\n", distancia);
}

void buscar_por_tipo() {
    char tipo[50];
    printf("Digite o tipo de catástrofe: ");
    scanf(" %[^"]", tipo);

    int encontrados = 0;
    for (int i = 0; i < total_relat; i++) {
        for (int j = 0; j < relatores[i].total_relatos; j++) {
            Relato *r = &relatores[i].relatos[j];
            if (strcmp(r->tipo, tipo) == 0) {
                printf("\n[%d] %s - %s\nDescrição: %s\nRelator: %s\nCoordenadas: %.4f, %.4f\n",
                       j + 1, r->data, r->hora, r->descricao, relatores[i].nome,
                       r->latitude, r->longitude);
                encontrados++;
            }
        }
    }

    if (encontrados == 0) {
        printf("Nenhum relato encontrado para o tipo '%s'.\n", tipo);
    }
}

void buscar_por_periodo() {
    char inicio[11], fim[11];
    printf("Data início (DD/MM/AAAA): ");
    scanf(" %[^"]", inicio);
    printf("Data fim (DD/MM/AAAA): ");
    scanf(" %[^"]", fim);

    int encontrados = 0;
    for (int i = 0; i < total_relat; i++) {
        for (int j = 0; j < relatores[i].total_relatos; j++) {
            Relato *r = &relatores[i].relatos[j];
            if (strcmp(r->data, inicio) >= 0 && strcmp(r->data, fim) <= 0) {
                printf("\n[%d] %s - %s\nTipo: %s\nDescrição: %s\nRelator: %s\nCoordenadas: %.4f, %.4f\n",
                       j + 1, r->data, r->hora, r->tipo, r->descricao,
                       relatores[i].nome, r->latitude, r->longitude);
                encontrados++;
            }
        }
    }

    if (encontrados == 0) {
        printf("Nenhum relato encontrado no período informado.\n");
    }
}

void relatos_exemplo() {
    if (total_relat >= MAX_RELATORES) return;

    Relator exemplo = {
        "João da Silva", "123456789", "joao@email.com", "11999999999",
        -23.5505, -46.6333, .total_relatos = 0
    };

    Relato r1 = {"Enchente", "Rua completamente alagada", "25/05/2025", "14:00", -23.5510, -46.6340};
    Relato r2 = {"Deslizamento", "Barranco cedeu próximo à escola", "24/05/2025", "10:30", -23.5520, -46.6350};
    Relato r3 = {"Incêndio", "Incêndio em residência", "23/05/2025", "08:15", -23.5530, -46.6360};

    exemplo.relatos[exemplo.total_relatos++] = r1;
    exemplo.relatos[exemplo.total_relatos++] = r2;
    exemplo.relatos[exemplo.total_relatos++] = r3;

    relatores[total_relat++] = exemplo;
}

int main() {
    relatos_exemplo();
    int opcao;
    do {
        printf("\nMenu:\n");
        printf("1. Cadastrar relator\n");
        printf("2. Cadastrar relato\n");
        printf("3. Buscar por tipo\n");
        printf("4. Buscar por período\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: cadastrar_relator(); break;
            case 2: cadastrar_relato(); break;
            case 3: buscar_por_tipo(); break;
            case 4: buscar_por_periodo(); break;
            case 0: printf("Encerrando programa.\n"); break;
            default: printf("Opção inválida.\n");
        }
    } while (opcao != 0);

    return 0;
}

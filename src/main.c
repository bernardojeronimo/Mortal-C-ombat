#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HEALTH 999
#define MAX_STAMINA 1103
#define INIT_HEALTH 837
#define INIT_STAMINA 1091
#define NUM_ATTACKS 12
#define NUM_COMBOS 4

typedef struct Node {
    char ataque[20];
    int vida;
    int estamina;
    struct Node *next;
} Node;

typedef struct Jogador{
    int vida;
    int estamina;
    Node *historico;
    int comboespecial;
} Jogador;

typedef struct Combo{
    char sequence[20];
    int vida_damage;
    int stamina_cost;
    char identificar;
} Combo;


int gerador (int min ,int max){
    return (rand() % (max - min + 1)) + min;
}

void limpar_historico(Jogador *j) {
    Node *atual = j->historico;
    Node *prox;

    while (atual != NULL) {
        prox = atual->next;
        free(atual);
        atual = prox;
    }

   j->historico = NULL;
}


void adicionar_historico(Jogador *jogador, char *ataques) {
    Node *no = (Node*)malloc(sizeof(Node));
    no->vida = jogador->vida;
    no->estamina = jogador->estamina;
    strcpy(no->ataque, ataques);
    no->next = NULL;

    if (jogador->historico == NULL) {
        jogador->historico = no;
    } else {
        Node *ultimo = jogador->historico;
        while (ultimo->next != NULL) {
            ultimo = ultimo->next;
        }
        ultimo->next = no;
    }
}


int executar_combo(char* combo, Jogador* jcombo, Jogador* jperdevida) {
    Combo combos[] = {
        {"ARROZAO", 500, 500, '1'},
        {"DADBAD", 400, 400, '2'},
        {"STTEACC", 300, 300, '3'},
        {"TATAPAAA", 200, 200, '4'}
    };    

    for (int i = 0; i < NUM_COMBOS; i++) {
        if (strcmp(combo, combos[i].sequence) == 0 || combo[0] == combos[i].identificar) {
             if (jcombo == NULL || jperdevida == NULL) {
                // Apenas verificando se o combo é válido
                return 1;
            } else if (jcombo->estamina > 750) {
                jperdevida->vida -= combos[i].vida_damage;
                jcombo->estamina -= combos[i].stamina_cost;
                adicionar_historico(jcombo,combo);
                return 1; 
            } else {
                return -1; // Estamina insuficiente
            }
        }
    }
    return 0; // Combo invalido
}

void processar_ataques(char* ataques1, char* ataques2, Jogador* j1, Jogador* j2, char modo);

int comboespecial(char* combo, Jogador* j1, Jogador* j2,char modo, int id) {
    int x = 0;
    char letras[] = {"ZPAETRCBOM"};
    Combo combos[] = {
        {"ARROZAO", 500, 500, '1'},
        {"DADBAD", 400, 400, '2'},
        {"STTEACC", 300, 300, '3'},
        {"TATAPAAA", 200, 200, '4'}
    };

    if (sscanf(combo, "TARZANTABORDA%d", &x) == 1 && (j1 == NULL || j2 == NULL)) {
        return 1;
    }

    if (sscanf(combo, "TARZANTABORDA%d", &x) == 1) {
        Node *current = j1->historico;
        int tamanho = 0;

       while (current) {
        tamanho += executar_combo(current->ataque, NULL, NULL) == 1 ? 1 : strlen(current->ataque);
        current = current->next;
    }

    if (x > tamanho || (id == 2 && x + 10 > tamanho))
    {
        j1->vida = INIT_HEALTH;
        j2->vida = INIT_HEALTH;
        j1->estamina = INIT_STAMINA;
        j2->estamina = INIT_STAMINA;

        limpar_historico(j1);
        limpar_historico(j2);
        adicionar_historico(j1,letras);

        if (modo != 'S')
        {
            printf(">>> voltou %d ataques\n", x);
        }
        return -1;
    }
    


        for (int i = 0; i < x; i++) {
            current = j1->historico;
            Node *prev = NULL;
            while (current && current->next) {
                prev = current;
                current = current->next;
            }
            if (current) {
                int len = strlen(current->ataque);
                if (len > 1) {
                    current->ataque[len - 1] = '\0';
                } else {
                    if (prev) {
                        prev->next = NULL;
                    } else {
                        j1->historico = NULL;
                    }
                    free(current);
                }
            }
        }

        for (int i = 0; i < x; i++) {
            current = j2->historico;
            Node *prev = NULL;
            while (current && current->next) {
                prev = current;
                current = current->next;
            }
            if (current) {
                int len = strlen(current->ataque);
                if (len > 1) {
                    current->ataque[len - 1] = '\0';
                } else {
                    if (prev) {
                        prev->next = NULL;
                    } else {
                        j2->historico = NULL;
                    }
                    free(current);
                }
            }
        }

        current = j1->historico;
        Node *current2 = j2->historico;
        current = current->next;

        Jogador jogador1 = {INIT_HEALTH, INIT_STAMINA, NULL};
        Jogador jogador2 = {INIT_HEALTH, INIT_STAMINA, NULL};
        adicionar_historico(&jogador1,letras);


        while (current && current2)
        {
            processar_ataques(current->ataque,current2->ataque,&jogador1,&jogador2,'T');
            current = current->next;
            current2 = current2->next;
        }

        j1->vida = jogador1.vida;
        j1->estamina = jogador1.estamina;
        j2->vida = jogador2.vida;
        j2->estamina = jogador2.estamina;

    
        if (modo != 'S')
        {
            printf(">>> voltou %d ataques\n", x);
        }
        return -1;
    }

    return 0;
}



void imprimir_historico(Jogador *jogador) {
    Node *current = jogador->historico;
    int tamanho = 0;
    int count = 0;
    Combo combos[] = {
        {"ARROZAO", 500, 500, '1'},
        {"DADBAD", 400, 400, '2'},
        {"STTEACC", 300, 300, '3'},
        {"TATAPAAA", 200, 200, '4'}
    };

    while (current) {
        tamanho += executar_combo(current->ataque, NULL, NULL) == 1 ? 1 : strlen(current->ataque);
        current = current->next;
    }

    char *historico = (char *)malloc((tamanho + 1) * sizeof(char));
    historico[tamanho] = '\0';

    current = jogador->historico;
    count = 0;


    while (current) {
        if (executar_combo(current->ataque, NULL, NULL) == 1) {
            for (int i = 0; i < NUM_COMBOS; i++) {
                if (strcmp(current->ataque, combos[i].sequence) == 0) {
                    historico[count] = combos[i].identificar;
                    count++;
                    break;
                }
            }
        } else {
            int ataqueLen = strlen(current->ataque);
            for (int i = 0; i < ataqueLen; i++) {
                historico[count] = current->ataque[i];
                count++;
            }
        }
        current = current->next;
    }

    for (int i = (tamanho > 17 ? tamanho - 17 : 0); i < tamanho; i++) {
        if (historico[i] >= '1' && historico[i] <= '4') {
            for (int j = 0; j < NUM_COMBOS; j++) {
                if (historico[i] == combos[j].identificar) {
                    printf("%s", combos[j].sequence);
                    break;
                }
            }
        } else {
            printf("%c", historico[i]);
        }
    }
    printf("\n");

    free(historico);
}



int cheatcodes(char *codigo, Jogador* j1, Jogador* j2) {
    int x = 0;
    char letras[] = {"ZPAETRCBOM"};

    if (strncmp(codigo, "Modo-Jesus", 10) == 0) {
        j1->vida = INIT_HEALTH;
        j2->vida = INIT_HEALTH;
        j1->estamina = INIT_STAMINA;
        j2->estamina = INIT_STAMINA;

        limpar_historico(j1);
        limpar_historico(j2);

        adicionar_historico(j1, letras);
        return -1;
    } else if (sscanf(codigo, "Alt-F4%d", &x) == 1) {
        if (x > 0) {
            j1->estamina += x;
            if (j1->estamina > MAX_STAMINA) j1->estamina = MAX_STAMINA;
            return -1;
        }
    } else if (sscanf(codigo, "Kebab%d", &x) == 1) {
        if (x > 0) {
            j2->estamina += x;
            if (j2->estamina > MAX_STAMINA) j2->estamina = MAX_STAMINA;
            return -1;
        }
    } else if (sscanf(codigo, "Hiroshima%d", &x) == 1) {
        if (x > 0) {
            j1->vida += x;
            if (j1->vida > MAX_HEALTH) j1->vida = MAX_HEALTH;
            return -1;
        }
    } else if (sscanf(codigo, "Nood-Mode%d", &x) == 1) {
        if (x > 0) {
            j2->vida += x;
            if (j2->vida > MAX_HEALTH) j2->vida = MAX_HEALTH;
            return -1;
        }
    }
    return 0;
}



int ataque_valido(char *ataque) {
    int tamanho = strlen(ataque);
    int x = 0;

    if (ataque == NULL) {
        return 0;
    }

    if (strncmp(ataque, "Modo-Jesus", 10) == 0 || sscanf(ataque, "Alt-F4%d", &x) == 1 || sscanf(ataque, "Kebab%d", &x) == 1 || sscanf(ataque, "Hiroshima%d", &x) == 1 || sscanf(ataque, "Nood-Mode%d", &x) == 1) {
        return 1;
    }

    if (comboespecial(ataque,NULL,NULL,' ',0) == 1)
    {
        return 1;
    }
    
    if (executar_combo(ataque, NULL, NULL) == 1) {
        return 1;
    }

    if (strlen(ataque) > 5){
        return 0;
    }
    

    for (int i = 0; i < tamanho; i++) {
        switch (ataque[i]) {
            case 'Z': case 'P': case 'A': case 'E': case 'T': 
            case 'R': case 'C': case 'B': case 'O': case 'M': 
            case 'D': case ' ':
                break;
            default:
                return 0;
        }
    }
    return 1;
}


char* computador(Jogador* j1, Jogador* j2) {
    int tamanho = 0;
    int count = 0;
    int combo = 0;
    int x = 0;
    char* ataques = malloc(20 * sizeof(char));
    Node *current = j1->historico;
    Combo combos[] = {
        {"ARROZAO", 500, 500, '1'},
        {"DADBAD", 400, 400, '2'},
        {"STTEACC", 300, 300, '3'},
        {"TATAPAAA", 200, 200, '4'}
    };

    while (current) {
        tamanho += executar_combo(current->ataque, NULL, NULL) == 1 ? 1 : strlen(current->ataque); 
        current = current->next;
    }

    if (tamanho > 50 && !j2->comboespecial)
    {
       j2->comboespecial = 1;
       x = gerador(1,50);
       sprintf(ataques,"TARZANTABORDA%d",x);
       return ataques;
    }
    

    char* letras = malloc((tamanho + 1) * sizeof(char));
    current = j1->historico;
    letras[count] = '\0';

    while (current) {
        
        if (executar_combo(current->ataque, NULL, NULL) == 1) {
            for (int i = 0; i < NUM_COMBOS; i++) {
                if (strcmp(current->ataque, combos[i].sequence) == 0) {
                    letras[count] = combos[i].identificar;
                    count++;
                    break;
                }
            }
        } else {
            int ataquelen = strlen(current->ataque);
            for (int i = 0; i < ataquelen; i++) {
                letras[count] = current->ataque[i];
                count++;
            }
        }
        current = current->next;
    }

    for (int i = 0; i < 5; i++) {
        int posicao = gerador(0, tamanho - 1);
        while (letras[posicao] == ' ') {
            posicao = gerador(0, tamanho - 1);
        }
        char identificador = letras[posicao];
        if (identificador >= '1' && identificador <= '4'){
            for (int j = 0; j < NUM_COMBOS; j++) {
            if (identificador == combos[j].identificar) {
                combo = 1;
                strcpy(ataques, combos[j].sequence);
                count += strlen(combos[j].sequence);
                return ataques;
                break;
                }
            }
        }
       else{
            ataques[i] = identificador;
       }
       
    }
    if (!combo)
    {
        ataques[5] = '\0';
    }
    free(letras);
    
    
    return ataques;
}



void processar_ataques(char* ataques1, char* ataques2, Jogador* j1, Jogador* j2, char modo) {
    int len1 = strlen(ataques1);
    int len2 = strlen(ataques2);
    int max_len = (len1 > len2) ? len1 : len2;
    int idx1, idx2, m1, m2, resultado;
    int x = 0;
    int tabela[NUM_ATTACKS][NUM_ATTACKS] = {
        // Z,  P,  A,  E,  T,  R,  C,  B,  O,  M,  D,  " "
        {  0,   4,   8,  12,  16, -20, -16,  12,  -8,  -4,  0, 20 },
        { -4,   0,   4,   8,  12,  16, -20, -16, -12,  16,  0, 20 },
        { -8,  -4,   0,   4,   8,  12,  16, -20, -16,  12,  0, 20 },
        {-12,  -8,  -4,   0,   4,  -8,  12,  16,  20, -16,  0, 20 },
        {-16, -12,  -8,  -4,   0,   4,  -8,  12,  16,  20,  0, 20 },
        { 20, -16, -12,   8,  -4,   0,   4,   8,  12, -16,  0, 20 },
        { 16,  20, -16, -12,   8,  -4,   0,  -8,  -4,  -8,  0, 20 },
        {-12,  16,  20, -16, -12,  -8,   8,   0,  16, -20,  0, 20 },
        {  8,  12,  16, -20, -16, -12,   4, -16,   0,  20,  0, 20 },
        {  4, -16, -12,  16, -20,  16,   8,  20, -20,   0,  0, 20 },
        {  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  0,  0 },
        {-20, -20, -20, -20, -20, -20, -20,  -20, -20, -20, 0,  0 }
    };

    m1 = (j1->estamina <= 250) ? 4 : (j1->estamina <= 500) ? 3 : (j1->estamina <= 750) ? 2 : 1;
    m2 = (j2->estamina <= 250) ? 4 : (j2->estamina <= 500) ? 3 : (j2->estamina <= 750) ? 2 : 1;


    if (comboespecial(ataques1,NULL,NULL,modo,1) == 1)
    {
        comboespecial(ataques1,j1,j2,modo,1);
        return;
    } else if (comboespecial(ataques2,NULL,NULL,modo,2) == 1)
    {
        comboespecial(ataques2,j1,j2,modo,2);
        return;
    }
    

    
    if (executar_combo(ataques1,NULL,NULL) == 1 && executar_combo(ataques2, NULL,NULL) == 1)
    {
        executar_combo(ataques1, j1, j2);
        executar_combo(ataques2, j2, j1);
        if (modo != 'S' && modo != 'T')
        {
            printf("[%s,%s]", ataques1,ataques2);
            printf("\n");
        }
        return;
    } else if (executar_combo(ataques1, NULL, NULL) == 1) {
        executar_combo(ataques1, j1, j2);
        j2->estamina += 25;
        if (j2->estamina > MAX_STAMINA) j2->estamina = MAX_STAMINA;
        ataques2[0] = ' ';
        ataques2[1] = '\0';
        adicionar_historico(j2,ataques2);
        if (modo != 'S' && modo != 'T')
        {
            printf("[%s, ]", ataques1);
            printf("\n");
        }
        return;
    } else if (executar_combo(ataques2, NULL, NULL) == 1) {
        executar_combo(ataques2, j2, j1);
        j1->estamina += 25;
        if (j1->estamina > MAX_STAMINA) j1->estamina = MAX_STAMINA;
        ataques1[0] = ' ';
        ataques1[1] = '\0';
        adicionar_historico(j1,ataques1);
        if (modo != 'S' && modo != 'T')
        {
              printf("[ ,%s]", ataques2);
            printf("\n");
        }
        return;
    }
    
    if (!ataque_valido(ataques1) || !ataque_valido(ataques2)) {
        printf("Entrada invalida\n");
        exit(0);
    }

    for (int i = len1; i < max_len; ++i) {
        ataques1[i] = ' ';
    }
    for (int i = len2; i < max_len; ++i) {
        ataques2[i] = ' ';
    }

    ataques1[max_len] = '\0';
    ataques2[max_len] = '\0';

    if (modo != 'S' && modo != 'T')
    {  
    for (int i = 0; i < max_len; i++) {
        printf("[%c,%c]", ataques1[i], ataques2[i]);
    }
    printf("\n");
    }

    for (int i = 0; i < max_len; i++) {
        idx1 = (ataques1[i] == 'Z') ? 0 :
               (ataques1[i] == 'P') ? 1 :
               (ataques1[i] == 'A') ? 2 :
               (ataques1[i] == 'E') ? 3 :
               (ataques1[i] == 'T') ? 4 :
               (ataques1[i] == 'R') ? 5 :
               (ataques1[i] == 'C') ? 6 :
               (ataques1[i] == 'B') ? 7 :
               (ataques1[i] == 'O') ? 8 :
               (ataques1[i] == 'M') ? 9 :
               (ataques1[i] == 'D') ? 10 :
               (ataques1[i] == ' ') ? 11 : 11;

        idx2 = (ataques2[i] == 'Z') ? 0 :
               (ataques2[i] == 'P') ? 1 :
               (ataques2[i] == 'A') ? 2 :
               (ataques2[i] == 'E') ? 3 :
               (ataques2[i] == 'T') ? 4 :
               (ataques2[i] == 'R') ? 5 :
               (ataques2[i] == 'C') ? 6 :
               (ataques2[i] == 'B') ? 7 :
               (ataques2[i] == 'O') ? 8 :
               (ataques2[i] == 'M') ? 9 :
               (ataques2[i] == 'D') ? 10 :
               (ataques2[i] == ' ') ? 11 : 11;

        resultado = tabela[idx1][idx2];

        if (resultado > 0) {
            j2->vida -= resultado * m2;
        } else if (resultado < 0) {
            j1->vida += resultado * m1;
        }

        j1->estamina -= (ataques1[i] != 'D') ? 23 : -7;
        j2->estamina -= (ataques2[i] != 'D') ? 23 : -7;

        if (ataques1[i] == 'D') {
            j1->vida += 13 * m1;
            if (j1->vida > MAX_HEALTH) j1->vida = MAX_HEALTH;
            if (j1->estamina > MAX_STAMINA) j1->estamina = MAX_STAMINA;
        }

        if (ataques2[i] == 'D') {
            j2->vida += 13 * m2;
            if (j2->vida > MAX_HEALTH) j2->vida = MAX_HEALTH;
            if (j2->estamina > MAX_STAMINA) j2->estamina = MAX_STAMINA;
        }

        if (ataques1[i] == ' ') {
            j1->estamina += 48;
            if (j1->estamina > MAX_STAMINA) j1->estamina = MAX_STAMINA;
        }
        
        if (j1->vida < 0) j1->vida = 0;
        if (j2->vida < 0) j2->vida = 0;
        if (j1->estamina < 0) j1->estamina = 0;
        if (j2->estamina < 0) j2->estamina = 0;

        if (j1->vida <= 0 && j2->vida <= 0) {
            printf("Empate!\n");
            exit(0);
        } else if (j1->vida <= 0) {
            printf("Jogador 2 venceu o jogo!\n");
            exit(0);
        } else if (j2->vida <= 0) {
            printf("Jogador 1 venceu o jogo!\n");
            exit(0);
        }
    }
        if(j1->vida > MAX_HEALTH) j1->vida = MAX_HEALTH;
        if(j2->vida > MAX_HEALTH) j2->vida = MAX_HEALTH;
        if(j2->estamina > MAX_STAMINA) j2->estamina = MAX_STAMINA;
        if(j1->estamina > MAX_STAMINA) j1->estamina = MAX_STAMINA;
        adicionar_historico(j1, ataques1);
        adicionar_historico(j2, ataques2);
}

void jogar_jogo(Jogador *jogador, char *id,char* ataque,char modo) {
    int m = (jogador->estamina <= 250) ? 4 : (jogador->estamina <= 500) ? 3 : (jogador->estamina <= 750) ? 2 : 1;


    if (modo != 'S') {
        printf("%s [%d|%d] (x%d)\n", id, jogador->vida, jogador->estamina, m);
        printf("H: ");
        imprimir_historico(jogador);
        if (ataque != NULL) {
            printf("I: %s\n", ataque);
        } else {
            printf("I: ");
        }
    }
}

int main(int argc, char *argv[]) {
    Jogador jogador1 = {INIT_HEALTH, INIT_STAMINA, NULL};
    Jogador jogador2 = {INIT_HEALTH, INIT_STAMINA, NULL};
    char letras[] = {"ZPAETRCBOM"};
    char ataque1[20], ataque2[20]; 
    unsigned int seed = 1;
    char sleep = ' ';

    adicionar_historico(&jogador1,letras);
    
    if (argc > 1) {
        seed = atoi(argv[1]);
        if (argc == 3)
        {
            sleep = argv[2][0];
        }
        
    }
    srand(seed);
    
    while (1) {

          if (jogador1.vida <= 0 && jogador2.vida <= 0) {
            printf("Empate!\n");
            return 0;
        } else if (jogador1.vida <= 0) {
            printf("Jogador 2 venceu o jogo!\n");
            return 0;
        } else if (jogador2.vida <= 0) {
            printf("Jogador 1 venceu o jogo!\n");
            return 0;
        }

        while (1) {
            jogar_jogo(&jogador1, "P#1", NULL,sleep);
            if (scanf("%s", ataque1) != 1 || !ataque_valido(ataque1)) {
                printf("Entrada invalida\n");
                exit(1);
            }
                 if (jogador1.estamina <= 750 && executar_combo(ataque1, &jogador1, &jogador2) == -1) {
                    if (sleep != 'S')
                    {
                        printf("Estamina insuficiente\n");
                    }
                    continue;       
                }
                else if (cheatcodes(ataque1,&jogador1,&jogador2) == -1){
                    continue;
                }
                else if (comboespecial(ataque1,&jogador1,&jogador2,sleep,1) == -1){
                    continue;
                }
                break;
        }

        while (1) {
            strcpy(ataque2, computador(&jogador1,&jogador2));
            jogar_jogo(&jogador2, "P#2", ataque2,sleep);
            if (ataque2 == NULL || !ataque_valido(ataque2)) {
                printf("Entrada invalida\n");
                exit(1);
            }
            if (jogador2.estamina <= 750 && executar_combo(ataque2, &jogador2, &jogador1) == -1) {
                if (sleep != 'S')
                {
                    printf("Estamina insuficiente\n");
                }
                continue;
            } 
            break;
        }
        processar_ataques(ataque1, ataque2, &jogador1, &jogador2,sleep);
    }
    return 0;
}
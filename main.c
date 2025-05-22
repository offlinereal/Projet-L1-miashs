#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>

#define MAX_PROMPT_SIZE 1000
#define MAX_INPUT_SIZE 100

// Structure pour gérer la réponse de l'API
struct string {
    char *ptr;
    size_t len;
};

void init_string(struct string *s) {
    s->len = 0;
    s->ptr = malloc(1);
    if (s->ptr == NULL) {
        fprintf(stderr, "Erreur mémoire\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s) {
    size_t new_len = s->len + size * nmemb;
    s->ptr = realloc(s->ptr, new_len + 1);
    if (s->ptr == NULL) {
        fprintf(stderr, "Erreur realloc\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s->ptr + s->len, ptr, size * nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;
    return size * nmemb;
}

int main() {
    system("chcp 65001 >nul");
    printf("Début du programme\n");
fflush(stdout);
    char surface[MAX_INPUT_SIZE], nb_pieces[MAX_INPUT_SIZE], localisation[MAX_INPUT_SIZE];
    char type_bien[MAX_INPUT_SIZE], annee[MAX_INPUT_SIZE], etat[MAX_INPUT_SIZE];
    char exterieur[MAX_INPUT_SIZE], stationnement[MAX_INPUT_SIZE], ascenseur[MAX_INPUT_SIZE];

    char prompt[MAX_PROMPT_SIZE] = "Peux-tu estimer le prix d’un bien immobilier";

    // Saisie utilisateur
    printf("Type de bien (appartement/maison) : ");
    scanf(" %[^\n]", type_bien);
    printf("Surface (en m²) : ");
    scanf(" %[^\n]", surface);
    printf("Nombre de pieces : ");
    scanf(" %[^\n]", nb_pieces);
    printf("Localisation (ville ou code postal) : ");
    scanf(" %[^\n]", localisation);
    printf("Annee de construction : ");
    scanf(" %[^\n]", annee);
    printf("Etat du logement (neuf, bon etat, a renover) : ");
    scanf(" %[^\n]", etat);
    printf("Presence d’un exterieur (balcon, terrasse, jardin) : ");
    scanf(" %[^\n]", exterieur);
    printf("Type de stationnement (garage, parking, aucun) : ");
    scanf(" %[^\n]", stationnement);
    printf("Presence d’un ascenseur (oui/non) : ");
    scanf(" %[^\n]", ascenseur);

    // Construction du prompt
    if (strcmp(type_bien, "-") != 0) {
        strcat(prompt, " de type ");
        strcat(prompt, type_bien);
    }
    if (strcmp(surface, "-") != 0) {
        strcat(prompt, " de ");
        strcat(prompt, surface);
        strcat(prompt, " m²");
    }
    if (strcmp(nb_pieces, "-") != 0) {
        strcat(prompt, " avec ");
        strcat(prompt, nb_pieces);
        strcat(prompt, " pieces");
    }
    if (strcmp(localisation, "-") != 0) {
        strcat(prompt, ", situe a ");
        strcat(prompt, localisation);
    }
    if (strcmp(annee, "-") != 0) {
        strcat(prompt, ", construit en ");
        strcat(prompt, annee);
    }
    if (strcmp(etat, "-") != 0) {
        strcat(prompt, ", en ");
        strcat(prompt, etat);
    }
    if (strcmp(exterieur, "-") != 0) {
        strcat(prompt, ", avec ");
        strcat(prompt, exterieur);
    }
    if (strcmp(stationnement, "-") != 0) {
        strcat(prompt, ", disposant d’un ");
        strcat(prompt, stationnement);
    }
    if (strcmp(ascenseur, "-") != 0) {
        strcat(prompt, ", avec ascenseur : ");
        strcat(prompt, ascenseur);
    }

    strcat(prompt, " ?");

    printf("\n---\nPrompt genere :\n%s\n", prompt);
    printf("---\n");

    // Envoi à l’API
    CURL *curl;
    CURLcode res;
    struct string response;

    const char *api_key = "sk-ta_clé_api";

    char data[2048];
    snprintf(data, sizeof(data),
        "{"
        "\"model\": \"gpt-3.5-turbo\","
        "\"messages\": ["
        "  {\"role\": \"system\", \"content\": \"Tu es un expert en estimation immobilière, donne moi une fourchette de prix.\"},"
        "  {\"role\": \"user\", \"content\": \"%s\"}"
        "]"
        "}", prompt);

    init_string(&response);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        struct curl_slist *headers = NULL;
        char auth_header[256];
        snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", api_key);

        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, auth_header);

        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) 
            fprintf(stderr, "Erreur : %s\n", curl_easy_strerror(res));
        else {
            printf("\n\033[1;34m═══════════════════════════════════════════════════════\033[0m\n");
            printf("\033[1;32m ESTIMATION IMMOBILIÈRE POUR VOTRE BIEN :\033[0m\n\n");
            printf("%s\n", response.ptr); // <- ou response.ptr si tu veux tout le JSON
            printf("\n\033[1;34m═══════════════════════════════════════════════════════\033[0m\n");
             } 

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    free(response.ptr);
    curl_global_cleanup();
    return 0;
}

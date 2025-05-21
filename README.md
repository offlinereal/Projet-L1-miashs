Prédiction du prix de bien immobilier

1 - Description : 

Projet réalisé lors du S2 en licence MIASHS à Paris X dans le cadre d'un projet de programmation assistée par l' IA . Ce projet est une application console en C qui permet d’estimer le prix d’un bien immobilier à partir de caractéristiques fournies par l’utilisateur .  
L’estimation est obtenue via une API fictive basée sur le modèle (GPT-3.5-turbo) d’OpenAI (chat completions), à laquelle on envoie un prompt construit dynamiquement .

2 - Fonctionnalités : 

- Saisie interactive des caractéristiques du bien immobilier (type, surface, nombre de pièces, localisation, année, état, etc.) .
- Tapez - si vous ne souhaitez pas renseigner une information . Le programme affichera le prompt généré et la réponse estimée de l’IA .
- Construction automatique d’un prompt détaillé à envoyer à l’API .
- Envoi du prompt à une API REST simulée via `libcurl` .
- Affichage de la réponse estimée (prix ou commentaires) retournée par l’IA .

3 - Prérequis : 

- Un compilateur C (`gcc`, `clang`, etc.)
- La bibliothèque **libcurl** installée
- **Clé API OpenAI** valide  
  La clé API doit être insérée dans le code source, dans la variable :  
  ```c
  const char *api_key = " sk-cle_api ";

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <unistd.h>
#include "serialcom.h"

/* Fonction envoieMsg(char *msg, char *port) :
 * ===========================================
 * ==> msg  : correspond au message à envoyé par bluetooth
 * ==> port : correspond au port sur lequel est connecté notre arduino (COM3/4/...)
*/
int envoieMsg(char *msg, char *port){
   /* Declaration des variables et structures :
    * =========================================
    * HANDLE         -> 
    * DCB            -> "Data Control Block" parametres pour le serial port 
   */
   HANDLE hSerial;
   DCB dcbSerialParams = {0}; 
   COMMTIMEOUTS timeouts = {0};

   // Open the highest available serial port number
   fprintf(stderr, "Ouverture du port...");
   hSerial = CreateFile(
         port, 
         GENERIC_READ|GENERIC_WRITE, 
         0, 
         NULL,
         OPEN_EXISTING, 
         FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
         NULL );

   if (hSerial == INVALID_HANDLE_VALUE)
   {
      fprintf(stderr, "Erreur port...\n");
      return 1;
   }
   else fprintf(stderr, "Connexion faite !\n");

   // Set device parameters (38400 baud, 1 start bit,
   // 1 stop bit, no parity)
   dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
   if (GetCommState(hSerial, &dcbSerialParams) == 0)
   {
      fprintf(stderr, "Erreut arduino..\n");
      CloseHandle(hSerial);
      return 1;
   }

   dcbSerialParams.BaudRate = CBR_38400;
   dcbSerialParams.ByteSize = 8;
   dcbSerialParams.StopBits = ONESTOPBIT;
   dcbSerialParams.Parity = NOPARITY;
   if(SetCommState(hSerial, &dcbSerialParams) == 0)
   {
      fprintf(stderr, "Erreur paramètres connexion...\n");
      CloseHandle(hSerial);
      return 1;
   }

   // Set COM port timeout settings
   timeouts.ReadIntervalTimeout = 50;
   timeouts.ReadTotalTimeoutConstant = 50;
   timeouts.ReadTotalTimeoutMultiplier = 10;
   timeouts.WriteTotalTimeoutConstant = 50;
   timeouts.WriteTotalTimeoutMultiplier = 10;
   if(SetCommTimeouts(hSerial, &timeouts) == 0)
   {
      fprintf(stderr, "Erreur timeout\n");
      CloseHandle(hSerial);
      return 1;
   }

   // Send specified text (remaining command line arguments)
   DWORD bytes_written, total_bytes_written = 0;
   fprintf(stderr, "Envoie du message...");
   if(!WriteFile(hSerial, msg, 50, &bytes_written, NULL))
   {
      fprintf(stderr, "Erreur\n");
      CloseHandle(hSerial);
      return 1;
   }
   fprintf(stderr, "%d envoyé !\n", bytes_written);

   // Close serial port
   fprintf(stderr, "Fermeture serial port...");
   if (CloseHandle(hSerial) == 0)
   {
      fprintf(stderr, "Erreur\n");
      return 1;
   }
   fprintf(stderr, "Ferme !\n");
}

int main()
{
   //VARIABLES qui ne changeront pas
   char msg_on[6] = "on\r\n";
   char msg_off[7] = "off\r\n";
   char term;
   int OPTION=-1, SOUSOPTION=-1;

   //VARIABLES qui peut changer
   char port[10] = "COM4";
   char nomAppairage[50] = "user_BasicUser_";
   int heureDiff = 15;
   int minutesDiff = 45;
      
   //MENU
   while(OPTION != 0){
      /* OPTION 1 :
       * ==========
       * Ce menu permet de configurer les donnees qui peuvent changer. Il y a un sous menu qui permet :
       * 1 -> se creer un nom qui sera afficher sur l'écran LCD pour confirmer la connection
       * 2 --> choisir l'heure de diffusion hebdomadaire
       * 3 ---> renseigner le port bluetooth sur lequel est connecté l'arduino (COM3/4/...)
      */
      if(OPTION == 1){
         while(SOUSOPTION!=0){
            if(SOUSOPTION == 1){
               char name[25];
               char debUser[50] = "user_";
               printf("Veuiller entrer votre nouveau nom d'appairage : ");
               scanf("%s", &name);
               strcat(debUser, name); //concatenation
               strcat(debUser, "_"); //concatenation
               strcpy(nomAppairage, debUser); //copie
               printf("Nouveau nom ==> %s\n", nomAppairage);
            }
            if(SOUSOPTION == 2){
               printf("Veuiller choisir l'heure de diffusion (ex: 15:45) : ");
               while(scanf("%d%c%d",&heureDiff, &term, &minutesDiff) != 3 || term != ':' || heureDiff > 23 || heureDiff < 0 || minutesDiff > 59 || minutesDiff < 0){
                  printf("Veuiller choisir l'heure de diffusion (ex: 15:45) : ");
                  while(getchar() != '\n');
               }
               printf("Vous avez choisi unr heure de %d:%d\n", heureDiff, minutesDiff);
            }
            if(SOUSOPTION == 3){
               char portInsert[25];
               printf("Veuiller entrer votre port (generalement COM3/COM4) : ");
               scanf("%s", &portInsert);
               strcpy(port, portInsert);
               printf("Nouveau port ==> %s\n", port);
            }

            printf("=========================================\n");
            printf("=======   CONFIGURATIONS DONNEES  =======\n");
            printf("=========================================\n");
            printf("|-> 0 - Quitter \n");
            printf("|--> 1 - Nom d'appairage \n");
            printf("|---> 2 - Heure de diffusion \n");
            printf("|----> 3 - Port de connexion \n");
            printf("=========================================\n");
            printf("Saisir une sous-option : ");
         
            while(scanf("%d%c",&SOUSOPTION, &term) != 2 || term != '\n')
            {
               printf("=========================================\n");
               printf("=======   CONFIGURATIONS DONNEES  =======\n");
               printf("=========================================\n");
               printf("|-> 0 - Quitter \n");
               printf("|--> 1 - Nom d'appairage \n");
               printf("|---> 2 - Heure de diffusion \n");
               printf("|----> 3 - Port de connexion \n");
               printf("=========================================\n");
               printf("Saisir une sous-option : ");
               while(getchar() != '\n');
            }
         }
      }

      /* OPTION 2 :
       * ==========
       * Cette fonctionnalité permet de s'assurer qu'on est bien connecté au diffuseur
       * 1 -> Vérifier que le port est bien renseigné
       * 2 --> Envoie le message et le port à la fonction envoieMsg qui s'occupe du reste
      */
      else if(OPTION == 2){
         if(port == "" || port == NULL){
            printf("Veuillez configurer un port valide..");
         }else{
            int retourErreur = envoieMsg(nomAppairage, port);
            printf("Pas encore implemente..");
         }
      }

      /* OPTION 3 :
       * ==========
       * Cette fonctionnalité permet de mettre à jours l'heure de diffusion pour le programme
       * 1 -> Définir le message qui va etre transmit à l'arduino contenant la nouvelle heure
       * 2 --> Envoie le message et le port à la fonction envoieMsg qui s'occupe du reste
      */
      else if(OPTION == 3){
         char heure[20] = "heures_";
         char minutes[20] = "minutes_";

         char heuresTransi[2];
         sprintf(heuresTransi, "%d", heureDiff);
         strcat(heure, heuresTransi);
         strcat(heure, "_");
         int retourErreurHeure = envoieMsg(heure, port);

         char MinutesTransi[2];
         sprintf(MinutesTransi, "%d", minutesDiff);
         strcat(minutes, MinutesTransi);
         strcat(minutes, "_");
         int retourErreurMinutes = envoieMsg(minutes, port);
         printf("Pas encore implemente..");
      }

      /* OPTION 4 :
       * ==========
       * Cette fonctionnalité permet d'actionner le diffuseur un coup
       * 1 -> Vérifier que le port est bien renseigné
       * 2 --> Envoie le message et le port à la fonction envoieMsg qui s'occupe du reste
      */
      else if(OPTION == 4){
         if(port == "" || port == NULL){
            printf("Veuillez configurer un port valide..");
         }else{
            int retourErreurPshit = envoieMsg("pshit_", port);
            printf("Pas encore implemente..");
         }
      }

      printf("\n================== MENU =================\n");
      printf("===    1 - CONFIGURATIONS DONNEES    ====\n");
      printf("===    2 - CONNEXION AU DIFFUSEUR    ====\n");
      printf("=== 3 - ENVOYER L'HEURE DE DIFFUSION ====\n");
      printf("===    4 - ACTIONNER LE DIFFUSEUR    ====\n");
      printf("=========================================\n");   
      printf("Saisir une option : ");
      SOUSOPTION=-1;

      while(scanf("%d%c",&OPTION, &term) != 2 || term != '\n'){
         printf("\n================== MENU =================\n");
         printf("===    1 - CONFIGURATIONS DONNEES    ====\n");
         printf("===    2 - CONNEXION AU DIFFUSEUR    ====\n");
         printf("=== 3 - ENVOYER L'HEURE DE DIFFUSION ====\n");
         printf("===    4 - ACTIONNER LE DIFFUSEUR    ====\n");
         printf("=========================================\n");
         printf("Saisir une option : ");
         while(getchar() != '\n');
      }
   }
   return 0;
}


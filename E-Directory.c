
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#define fName "records.txt"
#define MAX_LEN 25
#define PASS "1234"  // Password: 1234

typedef struct record {
   char name [MAX_LEN];
   char email [MAX_LEN];
   char phoneNum [MAX_LEN];
} Record;

// This function will get all the information of structure Record from the user. Then, it will write it on a text file named: "records.txt".
// The user is able to continue adding records until he/she decides to stop it.
void add () {
   Record rec;
   while (1) {
      puts("Enter record infomation - enter 'x' as a name to exit");
      printf("Name: "); gets(rec.name);
      if (!strcmp(rec.name,"x") || !strcmp(rec.name,"X")) 
         return;
      printf("Email: "); gets(rec.email);
      printf("Phone Number: "); gets(rec.phoneNum);
   
      FILE *fp;
      if (fp = fopen(fName, "a")) {
         fwrite(&rec, sizeof(Record), 1, fp);
         fclose(fp);
         puts("Record added successfully");
      } 
      else {
         puts("Error writing in file"); 
         return;
      }
   }
}

// This function will read from the previous text file "records.txt", and display all records information.
void viewAll () {
   FILE *fp;
   Record rec;
   if (fp = fopen(fName, "r")) {
      while (fread(&rec, sizeof(Record), 1, fp)) {
         printf("Name: %-10s Email: %-20s Phone number: %s\n", rec.name, rec.email, rec.phoneNum);
      }
      fclose(fp);
   } 
   else puts("Error reading from file");
}

// This function will ask the user to enter a name to search for it in the records list.
// It will go through the "records.txt" searching for the user's input.
// The function will display a proper message in both cases: when found it or not found it.
// In the case: name was found, then the function will display the total number of matched records with the same name. 
// The function shall display the related record’s information.
// The user is be able to continue searching for another input until he/she decides to stop it.
void search () {
   FILE *fp;
   int c;
   Record rec;
   while(1) {
      if (fp = fopen(fName, "r")) {
         char input [MAX_LEN];
         puts("\nEnter name to search in records or 'x' to exit");
         gets(input);
         if (!strcmp(input,"x") || !strcmp(input,"X")) 
            break;
         c = 0;
         while (fread(&rec, sizeof(Record), 1, fp))        
            if (!strcmp(input,rec.name)) {
               printf("Name: %-10s Email: %-20s Phone number: %s\n", rec.name, rec.email, rec.phoneNum);
               c++;
            }
         fclose(fp);
         if (c) printf("Number of records with mactching name: %d\n", c);
         else puts("Record not found");
      }    
      else puts("Error reading from file");
   } 
}

// This function will ask the user to enter the name for the targeted record. (Note: name is the key to find a record).
// If name found, then the user is asked to re-enter all his/her information again and update the record in the file "records.txt".
void edit () {
   FILE *fp;
   Record rec;
   Record newrec;
   int flag = 1;
   if(fp = fopen(fName,"r+")) {
      puts("Enter the name of the old record:");
      gets(newrec.name);
      while(fread(&rec, sizeof(Record), 1, fp))
         if(!strcmp(newrec.name, rec.name)) {
            flag = 0;
            puts("Enter new name or x to keep it the same");
            gets(newrec.name);
            puts("Enter new email or x to keep it the same");
            gets(newrec.email);
            puts("Enter new phone number or x to keep it the same");
            gets(newrec.phoneNum);
            if(strcmp(newrec.name, "x"))
               strcpy(rec.name, newrec.name);
            if(strcmp(newrec.email, "x"))
               strcpy(rec.email, newrec.email);
            if(strcmp(newrec.phoneNum, "x"))
               strcpy(rec.phoneNum, newrec.phoneNum);
            fseek(fp, -sizeof(Record), SEEK_CUR);
            fwrite(&rec, sizeof(Record), 1,fp);
            puts("Record edited successfully");
            break;
         }
      fclose(fp);
      if (flag) puts("Record not found");
   }
   else puts("Error reading from file");
}

// This function will ask the user to enter the name for the targeted record. (Note: name is the key to find a record).
// If name found, then the function will delete this specific record from the file "records.txt".
void deleteR () {
   FILE *fp, *temp;
   Record rec;
   int flag = 1;
   char input [MAX_LEN];
   if((fp = fopen(fName,"r")) && (temp = fopen("temp.txt","w"))) {
      puts("Enter the name of the record you want to delete");
      gets(input);
      while (fread(&rec, sizeof(Record), 1, fp)) {
         if (strcmp(input,rec.name))
            fwrite(&rec, sizeof(Record), 1, temp);
         else flag = 0;
      }
      fclose(temp);
      fclose(fp);
      remove(fName);
      rename("temp.txt", fName);
      if (flag) puts("Record not found");
      else puts("Record deleted successfully");
   } 
   else puts("Error opening file");
}

void checkpass () {
   char input [MAX_LEN];
   printf("Password: ");
   gets(input);
   if (!strcmp(input, PASS))
      return;
   puts("Invild password, please retry");
   checkpass();
}

void displaytime () {
   time_t rawtime;
   struct tm *timeinfo;
   time(&rawtime);  
   timeinfo = localtime(&rawtime);
   printf("Date / Time: %s\n", asctime(timeinfo));
}

int main () {
   checkpass();
   puts("**** Wlcome ****");
   displaytime();
   int x;
   do {
      puts("\nWhat do you want to do?");
      puts("(1) - Add new record");
      puts("(2) - Search records");
      puts("(3) - View all reccords");
      puts("(4) - Edit record");
      puts("(5) - Delete record");
      puts("(0) - Exit");
      scanf("%d", &x);
      fflush(stdin);
      switch (x) {
         case 1: add(); 
            break;
         case 2: search(); 
            break;
         case 3: viewAll(); 
            break;
         case 4: edit(); 
            break;
         case 5: deleteR(); 
            break;
         case 0: puts("Bye!");
            break;
         default: puts("Invalid input, please retry");
      }
   } while (x);
   return 0; 
}

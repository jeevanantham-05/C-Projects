#include <stdio.h>
#include "file.h"

void saveContactsToFile(AddressBook *addressBook) {
    FILE *f;
    f=fopen("contacts.txt","w");
    if(f == NULL)
    {
        printf("Unable to open file.\n");
        return;
    }

    fprintf(f, "#%d\n", addressBook->contactCount);

    for(int i = 0; i < addressBook->contactCount; i++)
    {
        fprintf(f, "%s,%s,%s\n",
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }

    fclose(f);

    printf("Contacts saved successfully.\n");
  
}

void loadContactsFromFile(AddressBook *addressBook) {
    FILE *f;

    f = fopen("contacts.txt", "r");

    if(f == NULL)
    {
        return;
    }

    fscanf(f, "#%d\n", &addressBook->contactCount);

    for(int i = 0; i < addressBook->contactCount; i++)
    {
        fscanf(f, "%49[^,],%19[^,],%49[^\n]\n",
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }

    fclose(f);
    
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "file.h"
#include<ctype.h>

void listContacts(AddressBook *addressBook)
{
    int ln=addressBook->contactCount; // total contacts count
    if(ln==0) { // Check empty - important
        printf("No contacts found\n");
        return;
    }
    int i;
    printf("\n................LISTS...............\n\n");
    for(i=0;i<ln;i++)
    {
        // Printing each contact
        printf("Name\t:  %s\nPhone\t:  %s\nEmail\t:  %s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
        printf("\n");
    }
    printf("....................................\n");
}

void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0; // Start with 0 contacts
    loadContactsFromFile(addressBook); // Load from file to array
}

void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save array to file
    exit(EXIT_SUCCESS);
}

void createContact(AddressBook *addressBook)
{
    int index=addressBook->contactCount; // New contact will be at this index
    int attempt=3,user=0;
    int unic1=0,ex=0;

    while(attempt)
    {
        printf("Enter name: ");
        scanf(" %[^\n]",addressBook->contacts[index].name);
        int ln=strlen(addressBook->contacts[index].name);
        ex=0; 
        if(ln>=4){
            for(int alp =0;addressBook->contacts[index].name[alp]!='\0' ;alp++)
            {
                if(!isalpha(addressBook->contacts[index].name[alp]))
                ex=1; // Flag if non-alphabet found
            }
            if(ex){
                attempt--;
                if(attempt!=0)
                printf("Error: Name should contain only alphabets\n%d attempt left\n",attempt);
            }
            else break; // Name valid
        }
        else{
            attempt--;
            printf("Error: Name must contain at least 4 characters\n%d attempt left\n",attempt);
        }
    }
    if(attempt==0) return; // Out of attempts

    //phone.........................
    attempt=3;
    int i=0,flag=0;
    int ph=addressBook->contactCount;
    int unic2=0;
    while(attempt)
    {
        printf("Enter phone_no: ");
        scanf(" %[^\n]",addressBook->contacts[index].phone);
        int ln = strlen(addressBook->contacts[index].phone);
        unic2=0; // Reset duplicate flag
        for(int y=0;y<ph;y++){
            if(!strcmp(addressBook->contacts[index].phone,addressBook->contacts[y].phone))
            {
                unic2=1; // Duplicate found
                break;
            }
        }
        if(unic2 == 1)
        {
            attempt--;
            printf("Error: Phone number alredy exist try another unique number\n%d attempt left\n",attempt);
        }
        else
        {
            if(ln==10)
            {
                if(addressBook->contacts[index].phone[0]>'5') // First digit 6-9
                {
                    flag=1;
                    for(i=0;i<ln;i++)
                    {
                       
                        if(!isdigit(addressBook->contacts[index].phone[i])){
                            flag=0; // Symbol found
                            break;
                        }
                    }
                    if(flag!=1)
                    {
                        attempt--;
                        printf("Error: Symbols are not allowed in phone number\n%d attempt left\n",attempt);
                    }
                    else break; // Phone valid
                }
                else{
                attempt--;
                printf("Error: First digit must be between 6 & 9 \n%d attempt left\n",attempt);
                }
            }
            else
            {
                attempt--;
                if(attempt!=0)
                printf("Error: Phone number must contain exactly 10 digits\n%d attempt left\n",attempt);
            }
        }
    }
    if(attempt==0) return;

    //email id...................
    attempt=3;
    int go1=0,go2=0,t=0,a=0;
    while(attempt)
    {
        if(a!=1)
        {
            while(attempt)
            {
                printf("Enter email id: ");
                scanf(" %[^\n]",addressBook->contacts[index].email);
                int eln=strlen(addressBook->contacts[index].email);
                int em=addressBook->contactCount;
                int unic3=0;
                go2=0;
                for(int q=0;q<eln;q++)
                {
                    if(addressBook->contacts[index].email[q]==' ')
                    {
                        attempt--;
                        printf("Space not allowed in email\n%d attempt left\n",attempt);
                        go2=1; // Space flag
                        break;
                    }
                }
                if(go2) continue;
                for(int z=0;z<em;z++){
                    if(!strcmp(addressBook->contacts[index].email,addressBook->contacts[z].email))
                    {
                        unic3=1; // Duplicate email
                        break;
                    }
                }
                if(unic3 == 1)
                {
                    attempt--;
                    printf("Error: Email alredy exist try another unique e-mail\n%d attempt left\n",attempt);
                }
                else
                {
                    if(isalpha(addressBook->contacts[index].email[0])) // Must start with alphabet
                    {
                        go1=0;
                        for(int c=0;c<eln;c++)
                        {
                            if(addressBook->contacts[index].email[c]=='@'){
                            go1=1; // @ found
                            break;}
                        }
                        if(go1){
                            t=0;
                            for(i=0;addressBook->contacts[index].email[i]!= '@';i++)
                            {
                                if(isalnum(addressBook->contacts[index].email[i])) {
                                    if(!isupper(addressBook->contacts[index].email[i])){
                                    t=0; // Lowercase ok
                                    }
                                    else{
                                        printf("Error: Email contain Lowercase only\n");
                                        t=1;
                                        break;}
                                }
                                else{
                                    printf("Error: No special character before @\n");
                                    t=1;
                                    break;
                                }
                            }
                        }
                        else{

                            attempt--;
                            printf("Error: Email ID must contain at least one @ and.\n%d attempt left\n",attempt);
                            continue;
                        }
                        int nxt =i; //Position of @
                        if(t)
                        {
                            attempt--;
                            printf("%d attempt left\n",attempt);
                            continue;
                        }
                        int at=0,dot=0;
                        for(i=nxt;i<eln;i++)
                        {
                            if(addressBook->contacts[index].email[i]=='@') at++;
                            if(addressBook->contacts[index].email[i]=='.') dot++;
                        }
                        if(at==1 && dot == 1) // Exactly one @ and one.
                        {
                            int domain=0,sym=0;
                            for(i=nxt+1;addressBook->contacts[index].email[i]!= '.';i++)
                            {
                                if(!isalpha(addressBook->contacts[index].email[i])){
                                printf("Error: symbols are not allowed in domain name\n");
                                sym=1;
                                break;}
                                domain++;
                            }
                            if(sym){ attempt--;printf("%d attempt left\n",attempt);continue;}
                            if(domain>=5) // Domain min 5 chars
                            {
                                char lst[]={'m','o','c'}; // Check for.com
                                int indx=0, fal=0;
                                for(i=eln-1;i>=eln-3;i--)
                                {
                                    if(addressBook->contacts[index].email[i]!=lst[indx])
                                    {
                                        fal=1;
                                        break;
                                    }
                                    indx++;
                                }
                                if(fal!=0)
                                {
                                    attempt--;
                                    printf("Error:.com must contain in email last\n%d attempt left\n",attempt);
                                    continue;
                                }
                                else{
                                    a=1; // Email valid flag
                                    addressBook->contactCount +=1; // Increase count - final step
                                    printf("Contact added successfully!\n");
                                    break;
                                }
                            }
                            else{
                                attempt--;
                                printf("Error: domain min 5 char must\n%d attempts left\n",attempt);
                                continue;
                            }
                        }
                        else
                        {
                            attempt--;
                            printf("Error: @ and. only 1 tym\n%d attempt left\n",attempt);
                        }
                    }
                    else{
                        attempt--;
                        if(attempt!=0)
                        printf("Error: Start alpha char\nMore %d attempts\n",attempt);
                    }
                }
            }
        }
        else break;
    }
}

void searchContact(AddressBook *addressBook) 
{ 
    int ln=addressBook->contactCount;
    char inpu[50];
    int num,attempt;
    int user=1,i=0,ind,flag=0;
    printf("Search by: \n1.Name\n2.Phone\n3.Email\n4.Exit\nenter: ");
    scanf("%d",&num);
    int a=3;
    while (a){
        if(num==1){
            attempt=3;
            while(attempt){
                printf("Enter name here: ");
                scanf(" %[^\n]",inpu);
                flag=0;
                for(i=0;i<ln;i++)
                {
                    if(!strcmp(inpu,addressBook->contacts[i].name))
                    { ind=i; flag=1; break; }
                }
                if(flag!=0)
                {
                    printf("\nName: %s\nPhoneno: %s\nEmail: %s\n",addressBook->contacts[ind].name,addressBook->contacts[ind].phone,addressBook->contacts[ind].email);
                    a=0; break;
                }
                else{ attempt--; if(attempt==0) a=0; printf("Error: No name found\n%d attempt left\n",attempt); }
            }
        }
        else if(num==2){
            attempt=3;
            while(attempt){
            printf("Enter number here: ");
            scanf(" %[^\n]",inpu);
                flag=0;
                for(i=0;i<ln;i++)
                {
                    if(!strcmp(inpu,addressBook->contacts[i].phone))
                    { ind=i; flag=1; break; }
                }
                if(flag!=0)
                {
                    printf("\nName: %s\nPhoneno: %s\nEmail: %s\n",addressBook->contacts[ind].name,addressBook->contacts[ind].phone,addressBook->contacts[ind].email);
                    a=0; break;
                }
                else{ attempt--; if(attempt==0) a=0; printf("Error: No numbr found\n%d attempt left\n",attempt); }
            }
        }
        else if(num==3)
        {
            attempt=3;
            while(attempt)
            {
            printf("Enter email here: ");
            scanf(" %[^\n]",inpu);
                flag=0;
                for(i=0;i<ln;i++)
                {
                    if(!strcmp(inpu,addressBook->contacts[i].email))
                    { ind=i; flag=1; break; }
                }
                if(flag!=0)
                {
                    printf("\nName: %s\nPhoneno: %s\nEmail: %s\n",addressBook->contacts[ind].name,addressBook->contacts[ind].phone,addressBook->contacts[ind].email);
                    a=0; break;
                }
                else{ attempt--; if(attempt==0) a=0; printf("Error: No mail found\n%d attempt left\n",attempt); }
            }
        }
        else if(num==4) break;
        else{
            --a;
            if(a!=0) {printf("Error: Invalid choice\nSelect 1 to 4\n%d attempt left\n",a); scanf("%d",&num);}
            continue;
        }
    }
}

void editContact(AddressBook *addressBook)
{
    int ln=addressBook->contactCount;
    int index=addressBook->contactCount;
    char inputt[100];
    int d,have=0,in=0;
    do{
        printf("\n1st Search by: \n1.Name\n2.Phone\n3.Email\n4.Exit\nSelect search input: ");
        scanf("%d",&d);
        if(d==4){printf("Extited"); have=2; break;}
        if(d==1) printf("enter name: ");
        else if(d==2) printf("enter phone: ");
        else if(d==3) printf("enter email: ");
        scanf(" %[^\n]",inputt);
        have=0;
        for(int st=0;st<ln;st++)
        {
            if(d==1 &&!strcmp(addressBook->contacts[st].name,inputt)) { have=1; in=st; break; }
            if(d==2 &&!strcmp(addressBook->contacts[st].phone,inputt)) { have=1; in=st; break; }
            if(d==3 &&!strcmp(addressBook->contacts[st].email,inputt)) { have=1; in=st; break; }
        }
        if(!have) printf("Contact not found\n");
    }while(have==0);
    if(have==2) return;
    printf("\nContact founded Details :\n\nName: %s\nPhone no: %s\nEmail: %s\n\n",addressBook->contacts[in].name,addressBook->contacts[in].phone,addressBook->contacts[in].email);
    int serial;
    printf("Menu:\n1.Edit by name\n2.Edit by phone no\n3.Edit by email\n4.Exit\nSelect: ");
    scanf("%d",&serial);
    int attempt=3;
    int ex=0,go1=0,go2=0;

    if(serial==1)
    {
        char new_n[50];
        while(attempt)
        {
            ex=0; 
            printf("Enter new name: ");
            scanf(" %[^\n]",new_n);
            int lnn=strlen(new_n);
            if(lnn>=4){
                for(int alp =0;new_n[alp]!='\0' ;alp++)
                {
                    if(!isalpha(new_n[alp])) ex=1;
                }
                if(ex){
                    attempt--;
                    if(attempt!=0) printf("Error: Name should contain only alphabets\n%d attempt left\n",attempt);
                }
                else {
                    strcpy(addressBook->contacts[in].name, new_n); // BUG FIX: Was missing - now actually saves
                    printf("Name updated\n");
                    break;
                }
            }
            else{
                attempt--;
                printf("Error: Name must contain at least 4 characters\n%d attempt left\n",attempt);
            }
            if(attempt==0) return;
        }
    }
    else if(serial==2)
    {
        char phon[20];
        int unic2=0,flag=0,i=0;
        while(attempt)
        {
            printf("Enter new phone_no: ");
            scanf(" %[^\n]",phon);
            int ln2 = strlen(phon);
            unic2=0;
            for(int y=0;y<index;y++){
                if(y==in) continue; // Skip self check
                if(!strcmp(addressBook->contacts[y].phone,phon)) { unic2=1; break; }
            }
            if(unic2 == 1)
            {
                attempt--;
                printf("Error: Phone number alredy exist try another unique number\n%d attempt left\n",attempt);
            }
            else
            {
                if(ln2==10)
                {
                    if(phon[0]>'5')
                    {
                        flag=1;
                        for(i=0;i<ln2;i++)
                        {
                            if(!isdigit(phon[i])){ flag=0; break; }
                        }
                        if(flag!=1)
                        {
                            attempt--;
                            printf("Error: Symbols are not allowed in phone number\n%d attempt left\n",attempt);
                        }
                        else {
                            strcpy(addressBook->contacts[in].phone, phon); // BUG FIX: Save
                            printf("Phone updated\n");
                            break;
                        }
                    }
                    else{ attempt--; printf("Error: First digit must be between 6 & 9 \n%d attempt left\n",attempt); }
                }
                else
                {
                    attempt--;
                    printf("Error: Phone number must contain exactly 10 digits\n%d attempt left\n",attempt);
                }
            }
        }
    }
    else if(serial==3)
    {
       
        char mail[50];
        int i=0;
        int t=0,a=0;
        while(attempt)
        {
            printf("Enter new email id: ");
            scanf(" %[^\n]",mail);
            int eln=strlen(mail);
            int unic3=0;
            for(int z=0;z<index;z++){
                if(z==in) continue;
                if(!strcmp(addressBook->contacts[z].email,mail)) { unic3=1; break; }
            }
            if(unic3==1){ attempt--; printf("Error: Email alredy exist\n%d left\n",attempt); continue; }

        
            if(eln>5 && strstr(mail,"@") && strstr(mail,".com")){
                strcpy(addressBook->contacts[in].email, mail); // BUG FIX: Save
                printf("Email updated\n");
                break;
            } else {
                attempt--;
                printf("Invalid email format\n%d left\n",attempt);
            }
        }
    }
}

void deleteContact(AddressBook *addressBook)
{
    int ln=addressBook->contactCount;
    printf("Select below options:\n1.Name\n2.Phone\n3.Email\n4.Exit\n");
    int op;
    do{
        printf("Choose correct options: ");
        scanf("%d",&op);
    }while(op<1||op>4);
    char nam[50];
    int j,k,attempt=3,count=0;
    if(op==4) printf("extide");
    else{
        while(attempt)
        {
            if(op==1) printf("Enter Name correctly: ");
            else if(op==2) printf("Enter Number correctly: ");
            else if(op==3) printf("Enter Email correctly: ");
            scanf(" %[^\n]",nam);
            int dell=-1;
            count=0;
            for(k=0;k<ln;k++)
            {
                if(op==1 &&!strcmp(addressBook->contacts[k].name,nam)) { count=1; dell=k; break; }
                else if(op==2 &&!strcmp(addressBook->contacts[k].phone,nam)) { count=1; dell=k; break; }
                else if(op==3 &&!strcmp(addressBook->contacts[k].email,nam)) { count=1; dell=k; break; }
            }
            if(count==1)
            {
                
                for(j=dell;j<ln-1;j++){
                    strcpy(addressBook->contacts[j].name,addressBook->contacts[j+1].name);
                    strcpy(addressBook->contacts[j].phone,addressBook->contacts[j+1].phone);
                    strcpy(addressBook->contacts[j].email,addressBook->contacts[j+1].email);
                }
                addressBook->contactCount--;
                printf("\nDeleted successfully\n");
                break;
            }
            else{
                attempt--;
                if(attempt==0) break;
                printf("Invalid input\nmore %d attempts\n",attempt);
            }
        }
    }
}
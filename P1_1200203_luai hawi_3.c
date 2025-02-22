#include <stdio.h>
#include <stdlib.h>
#include<strings.h>
//passenger node
struct passenger{
    int id;
    int date;
    char time[6];
    char from[20];
    char to[20];
    struct passenger* next;
};

//bus node
struct bus{
    int number;
    int date;
    char time[6];
    char from[20];
    char to[20];
    double price;
    int capacity;
    struct passenger* p;
    struct bus* next;
};

typedef struct passenger* pa;
typedef struct bus* bu;

pa passengers;
bu busses;

//functions definitions
pa MakeEmptyP(pa);
bu MakeEmptyB(bu);
void LoadPassengers();
void LoadBusses();
void InsertP(int,int,char[],char[],char[],pa,pa);
void InsertB(int,int,char[],char[],char[],double,int,bu,bu);
void InsertPToB(pa,pa,bu);
void assign();
void printAssignmentInfo();
int passengerAlreadyAssigned(int,pa);
int busAlreadyAssigned(int,bu);
int passengerAlreadyAssignedToBus(int ,bu );
void deletePassenger(int,pa);
void deletePassengerFromBus(int,bu);
void deleteBusNumber(int,bu);
pa findPreviousPassenger(int,pa);
bu findPreviousBus(int,bu);
int isLastP(pa);
int passengersCount(pa);
void deleteListP(pa);
void deleteListB(bu);
void unmatchedPassengers();
void addPassenger();
void printSpecificBus(int);
void save();
int menu();


int main()
{
    //creating the header for passengers and busses
    passengers=MakeEmptyP(passengers);
    busses=MakeEmptyB(busses);


    int choice=0;
    while(choice!=10){
        choice=menu();
        switch(choice){
            case 1:
                LoadBusses();
                break;
            case 2:
                LoadPassengers();
                break;
            case 3:
                assign();
                break;
            case 4:
                puts("enter bus number:");
                int number2;
                scanf("%d",&number2);
                printSpecificBus(number2);
                break;
            case 5:
                unmatchedPassengers();
                break;
            case 6:
                addPassenger();
                break;
            case 7:
                puts("enter id: ");
                int id;
                scanf("%d",&id);
                //deleting passenger from passengers linked list
                deletePassenger(id,passengers);
                //deleting passenger from passengers linked list in every bus node
                deletePassengerFromBus(id,busses);
                break;
            case 8:
                puts("enter bus number :");
                int number;
                scanf("%d",&number);
                deleteBusNumber(number,busses);
                break;
            case 9:
                save();
                break;
            default:
                break;
        }
    }
    return 0;
}

//memory allocation for headers
pa MakeEmptyP(pa L){
    if(L != NULL)
        deleteListP( L );
    L = (pa)malloc(sizeof(struct passenger));
    if(L == NULL)
        printf("Out of memory!\n");
    L->next = NULL;
    return L;
}
bu MakeEmptyB(bu L){
    if(L != NULL)
        deleteListB( L );
    L = (bu)malloc(sizeof(struct bus));
    if(L == NULL)
        printf("Out of memory!\n");
    L->next = NULL;
    return L;
}

//displaying the menu and returning choice to main
int menu(){
    printf("pic a number:\n");
    printf("1. Load the bus information file\n"
           "2. Load the passenger information file\n"
           "3. Assign passengers and print assignment information of all busses\n"
           "4. Print a specific bus information along with its passengers information (IDs)\n"
           "5. Print unmatched passengers\n"
           "6. Add new passenger\n"
           "7. Delete passenger\n"
           "8. Delete bus number\n"
           "9. save\n"
           "10. Exit\n");
    int choice;
    scanf("%d", &choice);
    if(choice>10||choice<1){
        printf("unavailable number\n");
        return -1;
    }
    return choice;
}

//reading from bus file and adding to busses limked list
void LoadBusses(){
    FILE* file=fopen("busses.txt", "r");
    if(file==NULL){
        puts("file does not exist...");
        return;
    }
    char data[100];
    int number,date,capacity;
    double price;
    char time[6],from[20],to[20];
    while(fgets(data,100,file)!=NULL){
        sscanf(data,"%d#%d#%[^#]#%[^#]#%[^#]#%lf#%d", &number,&date,time,from,to,&price,&capacity);
        if(!busAlreadyAssigned(number,busses))           //checks if bus already in busses linked list to not add it again when pressing on load busses twice
            InsertB(number,date,time,from,to,price,capacity,busses,busses);
    }
    fclose(file);
}

//reading passengers file and adding to passengers linked list
void LoadPassengers(){
    FILE* file=fopen("passengers.txt", "r");
    if(file==NULL){
        puts("file does not exist...");
        return;
    }
    char data[100];
    int id,date;
    char time[6],from[20],to[20];
    while(fgets(data,100,file)!=NULL){
        sscanf(data,"%d#%d#%[^#]#%[^#]#%[^#\n]", &id,&date,time,from,to);
        if(!passengerAlreadyAssigned(id,passengers))  //checks if passenger already in passengers linked list
            if(!passengerAlreadyAssignedToBus(id,busses))  //checks if passenger already in passengers linked list of every bus
                InsertP(id,date,time,from,to,passengers,passengers);
    }
    fclose(file);
}

//creating bus node and add it to busses linked list
void InsertB(int number,int date, char time[], char from[],char to[], double price, int capacity, bu l,bu p){
    bu temp;
    temp=(bu)malloc(sizeof(struct bus));
    temp->p=NULL;//this is needed for calling make empty to create passenger node ,if its not null make empty will call deleting list and using pointer->next when its not pointing to passenger node yet
    temp->p=MakeEmptyP(temp->p);//reserving memory for passenger node (header)to assign passengers to bus
    temp->number=number;
    temp->date=date;
    strcpy(temp->from,from);
    strcpy(temp->time,time);
    strcpy(temp->to,to);
    temp->price=price;
    temp->capacity=capacity;
    temp->next=p->next;
    p->next=temp;
}

//memory allocation for node of passenger and add it to passengers linked list
void InsertP(int id,int date, char time[], char from[],char to[], pa l,pa p){
    pa temp;
    temp=(pa) malloc(sizeof(struct passenger));
    temp->id=id;
    temp->date=date;
    strcpy(temp->from,from);
    strcpy(temp->time,time);
    strcpy(temp->to,to);
    temp->next=p->next;
    p->next=temp;
}

//assigning matched passengers to their busses in the header of passenger in each bus
void assign(){
    bu temp;
    pa temp2;
    temp=busses->next;
    //moving on each bus
    while(temp!=NULL){
        temp2=passengers->next;
        //moving on each passenger every time bus changes
        while(temp2!=NULL){
            if(!passengerAlreadyAssigned(temp2->id,temp->p)){  //checks if passenger already assigned to bus for more safty
                        //(not needed because when assigning to bus the node dissapears from passengers liked list to passengers liked list in the bus)
                if(temp->date==temp2->date&&strcmpi(temp->from,temp2->from)==0&&strcmpi(temp->to,temp2->to)==0){ //conditions for matching passenger(date,from,to, need to be equall in both sides)
                    //splitting time to hours and minutes
                    int hours,minutes,hours2,minutes2;
                    char* token;
                    char ti[6];
                    strcpy(ti,temp2->time);
                    token=strtok(ti,":");
                    hours=atoi(token);
                    token=strtok(NULL,":");
                    minutes=atoi(token);
                    strcpy(ti,temp->time);
                    token=strtok(ti,":");
                    hours2=atoi(token);
                    token=strtok(NULL,":");
                    minutes2=atoi(token);
                    if(hours<hours2||(hours==hours2&&minutes<=minutes2)){ //checks matching of time to passenger and bus(passenger can wait the bus but the bus can not wait him)
                        if(passengersCount(temp->p)<temp->capacity){  //checks if the capacity of the bus not full
                            if(temp2->next!=NULL){
                                temp2=temp2->next;
                                InsertPToB(findPreviousPassenger(temp2->id,passengers),passengers,temp); //moving the passenger node from the global passengers linked list to passengers linked list in the bus
                                continue;
                                }
                            else{
                                InsertPToB(temp2,passengers,temp);
                                break;
                            }
                        }
                        else
                            break;
                    }

                }

            }


            temp2=temp2->next;
        }
        temp=temp->next;
    }
    printAssignmentInfo();
}

//checking if the id of the passenger is in the linked list
int passengerAlreadyAssigned(int id, pa l){
    pa temp;
    temp=l->next;
    while (temp!=NULL){
        if(temp->id==id)
            return 1;
        temp=temp->next;
    }
    return 0;
}

//checks if bus number is in the linked list
int busAlreadyAssigned(int number, bu l){
    bu temp;
    temp=l->next;
    while (temp!=NULL){
        if(temp->number==number)
            return 1;
        temp=temp->next;
    }
    return 0;
}

//checks if passenger exist in any bus (passengers linked list in bus)
int passengerAlreadyAssignedToBus(int id, bu l){
    bu temp;
    pa temp2;
    temp=l->next;
    while (temp!=NULL){
        temp2=temp->p->next;
        while(temp2!=NULL){
            if(temp2->id==id)
                return 1;
            temp2=temp2->next;
        }
        temp=temp->next;
    }
    return 0;
}

//finding previous passenger node
pa findPreviousPassenger(int id,pa l){
    pa temp=l;
    while(temp->next!=NULL&&temp->next->id!=id){
        temp=temp->next;
    }
    return temp;
}

//checks if the node is the last one
int isLastP(pa p){
    return p->next==NULL;
}

//finding previous bus node
bu findPreviousBus(int number,bu l){
    bu temp=l;
    while(temp->next!=NULL&&temp->next->number!=number){
        temp=temp->next;
    }
    return temp;
}

//deleting passenger from list and memory
void deletePassenger(int id,pa l){
    pa temp, p;
    p=findPreviousPassenger(id,l);
    if(!isLastP(p)){
        temp=p->next;
        p->next=temp->next;
        free(temp);
    }
}

//displaying bus information and assigned passengers
void printAssignmentInfo(){
    bu temp=busses->next;
    pa temp2;
    while(temp!=NULL){
        printf("bus number: %d\tdate: %d\ttime: %s\ttrip from %s to %s\tprice: %.1lf\tcapacity: %d\tpassengers:\n",temp->number,temp->date,temp->time,temp->from,temp->to,temp->price,temp->capacity);
        temp2=temp->p->next;
        while(temp2!=NULL){
            printf("id: %d\n", temp2->id);
            temp2=temp2->next;
        }
        temp=temp->next;
    }
}

//counts nodes in passengers linked list
int passengersCount(pa l){
    int counter=0;
    pa temp=l->next;
    while(temp!=NULL){
        counter++;
        temp=temp->next;
    }
    return counter;
}

//deleting passenger list
void deleteListP(pa l){
    pa p,temp;
    p=l->next;
    l->next=NULL;
    while(p!=NULL){
        temp=p->next;
        free(p);
        p=temp;
    }
}

//deleting bus list
void deleteListB(bu l){
    bu p,temp;
    p=l->next;
    l->next=NULL;
    while(p!=NULL){
        temp=p->next;
        free(p);
        p=temp;
    }
}

//displaying the not assigned passengers (those who still there in global passengers linked list)
void unmatchedPassengers(){
    pa temp=passengers->next;
    while(temp!=NULL){
        printf("id: %d\tdate: %d\ttime: %s\tfrom: %s\tto: %s\n",temp->id,temp->date,temp->time,temp->from,temp->to);
        temp=temp->next;
    }
}

//moving passenger node from global list to passenger list in bus
void InsertPToB(pa p,pa l,bu b){
    pa k;
    k=findPreviousPassenger(p->id,l);
    if(!isLastP(k)){
        k->next=p->next;
        p->next=b->p->next;
        b->p->next=p;

    }

}

//adding new passenger
void addPassenger(){
    puts("enter passenger id: ");
    int id;
    scanf("%d",&id);
    puts("enter passenger travelling date: ");
    int date;
    scanf("%d",&date);
    puts("enter traveling time: ");
    getchar();
    char time2[6];
    while(1){
            char time[6];
            gets(time2);
            strcpy(time,time2);
            char* token;
            token=strtok(time,":");
            if(atoi(token)<=24&&atoi(token)>=0&&atoi(token)!=NULL){  //testing the validity of time(hours)
                token=strtok(NULL,":");
                if(atoi(token)<60&&atoi(token)>=0&&atoi(token)!=NULL)//testing the validity of time(minutes)
                    break;
            }
            printf("incorrect time\ntry again\n");
        }


    puts("enter passenger place: ");
    char from[20];
    gets(from);
    puts("enter passenger destination: ");
    char to[20];
    gets(to);
    InsertP(id,date,time2,from,to,passengers,passengers);
}

//searching of passenger in busses and deleting him
void deletePassengerFromBus(int id,bu l){
    bu temp=l->next;
    pa temp2;
    while(temp!=NULL){
        temp2=temp->p->next;
        while(temp2!=NULL){
            if(temp2->id==id){
                deletePassenger(id,temp->p);
                return;
            }
            temp2=temp2->next ;
        }
        temp=temp->next;
    }
}

//deleting bus from busses linked list and from the memory
void deleteBusNumber(int number,bu l){
    bu temp=l->next;
    pa temp2;
    while(temp!=NULL){  //moving on all busses
        if(temp->number==number){  //if bus is found code will empty all passengers from the bus and return them to global list of passengers
            temp2=temp->p->next;
            while(temp2!=NULL){
                pa temp3=passengers->next;
                pa temp4=temp2->next;
                passengers->next=temp2;
                temp2->next=temp3;
                temp->p->next=temp4;
                temp2=temp->p->next;
            }
            bu temp3=findPreviousBus(temp->number,busses);
            temp3->next=temp->next;
            free(temp->p);  //free to header in bus
            free(temp);  //free to bus node
            return;
        }
        temp=temp->next;
    }

}

//printing specific bus info with its passengers
void printSpecificBus(int number){
    bu temp=findPreviousBus(number,busses)->next;
    if(temp==NULL){      //checks if bus exists
        puts("bus does not exist...");
        return;
    }
    pa temp2=temp->p->next;
    printf("bus number: %d\tdate: %d\ttime: %s\ttrip from %s to %s\tprice: %lf\tcapacity: %d\tpassengers:\n",temp->number,temp->date,temp->time,temp->from,temp->to,temp->price,temp->capacity);
    while(temp2!=NULL){
            printf("id: %d\n", temp2->id);
            temp2=temp2->next;
        }
}
//opening files and writing the new data in them
void save(){
    FILE* pass=fopen("passengers.txt","w");
    FILE* buss=fopen("busses.txt","w");
    bu temp=busses->next;
    pa temp2;
    while(temp!=NULL){
        temp2=temp->p->next;
        while(temp2!=NULL){  //moving on every assigned passenger and printing their info in the file
            if(temp2->next!=NULL||passengersCount(passengers)!=0)
                fprintf(pass,"%d#%d#%s#%s#%s\n",temp2->id,temp2->date,temp2->time,temp2->from,temp2->to);
            else{
                fprintf(pass,"%d#%d#%s#%s#%s",temp2->id,temp2->date,temp2->time,temp2->from,temp2->to);
                break;
            }
            temp2=temp2->next;
        }


        if(temp->next!=NULL)
            fprintf(buss,"%d#%d#%s#%s#%s#%lf#%d\n",temp->number,temp->date,temp->time,temp->from,temp->to,temp->price,temp->capacity);
        else{
            fprintf(buss,"%d#%d#%s#%s#%s#%lf#%d",temp->number,temp->date,temp->time,temp->from,temp->to,temp->price,temp->capacity);
            break;
        }
        temp=temp->next;
    }
    temp2=passengers->next;
    while(temp2!=NULL){  //moving on the not assigned passengers and print their info in the file
        if(temp2->next!=NULL)
            fprintf(pass,"%d#%d#%s#%s#%s\n",temp2->id,temp2->date,temp2->time,temp2->from,temp2->to);
        else{
            fprintf(pass,"%d#%d#%s#%s#%s",temp2->id,temp2->date,temp2->time,temp2->from,temp2->to);
        }
        temp2=temp2->next;
    }
    fclose(pass);
    fclose(buss);
}

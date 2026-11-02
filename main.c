#include<stdio.h>
#include "definitions.h"
#include "function_definitions.h"
#include<string.h>

void next_line(FILE* file, char line[]){
    char c;
    line[0] = '\0';
    while(line[0]=='\0' && fgets(line, 256, file)!=NULL){
        if(sscanf(line, " %c", &c)!=1){
            line[0] = '\0';
        }
    }
}
 
void read_hardcoded_data(){
    FILE* file = fopen("hardcoded_data.txt", "r");
    if(file == NULL){
        printf("Could not open hardcoded_data.txt\n");
    }
    else{
        int total;
        int i;
        char line[256];
        char name[NAME_LENGTH];
        char contact[CONTACT_LENGTH];
        int price;
        int pincode;
        int customer_id;
        int service_id;
        date_time dt;
 
        next_line(file, line);
        if(sscanf(line, "%d", &total)!=1){
            total = 0;
        }
        for(i = 0; i<total; i++){
            next_line(file, line);
            if(sscanf(line, "%49s %d", name, &price)==2){
                addservice(name, price);
            }
        }
 
        next_line(file, line);
        if(sscanf(line, "%d", &total)!=1){
            total = 0;
        }
        for(i = 0; i<total; i++){
            next_line(file, line);
            if(sscanf(line, "%49s %d %10s", name, &pincode, contact)==3){
                registerCustomer(name, pincode, contact);
            }
        }
 
        next_line(file, line);
        if(sscanf(line, "%d", &total)!=1) total = 0;
        for(i = 0; i<total; i++){
            int used;
            next_line(file, line);
            if(sscanf(line, "%49s %d %10s%n", name, &pincode, contact, &used)==3){
                Service* services[MAX_SERVICES] = {NULL};
                int service_count = 0, id, len;
                char* p = line + used;
                while(service_count < MAX_SERVICES && sscanf(p, "%d%n", &id, &len)==1){
                    p = p + len;
                    services[service_count] = findService(id);
                    if(services[service_count]==NULL){
                        printf("%s: invalid service Id %d\n", name, id);
                    }
                    else{
                        service_count++;
                    }
                }
                if(service_count>0){
                    registerProfessional(name, pincode, contact, services);
                }
            }
        }
 
        next_line(file, line);
        if(sscanf(line, "%d", &total)!=1){
            total = 0;
        }
        for(i = 0; i<total; i++){
            next_line(file, line);
            if(sscanf(line, "%d %d %d %d %d %d %d", &customer_id, &service_id, &dt.day, &dt.month, &dt.year, &dt.hour, &dt.minute)==7){
                createServiceRequest(customer_id, service_id, dt);
            }
        }
        fclose(file);
    }
}

int main(){
    printf("=========================================================================\n");
    printf("--------- Welcome to URBAN COMPANY SERVICE MANAGEMENT SYSTEM ------------\n");
    printf("------------------- This project is developed by: -----------------------\n");
    printf("-------------------------- OM SWAPNIL GADE ------------------------------\n");
    printf("=========================================================================\n");
    int n;
    status_code loaded = load_all_data();
    if(loaded==FAILURE){
        printf("Loading the initial data from hardcoded_data.txt\n");
        read_hardcoded_data();
        save_all_data();
    }
    else{
        printf("Saved data loaded successfully\n");
    }
    do{
        printf("-----------------------------------------------\n");
        printf("Please select the type of user:\n");
        printf("1. Customer\n");
        printf("2. Professional\n");
        printf("3. Admin\n");
        printf("4. Exit\n");
        printf("-----------------------------------------------\n"); 
        printf("Enter your choice: "); 
        scanf("%d", &n);
        if(n==1){
            int m;
            do{
                printf("-----------------------------------------------\n");
                printf("Please select the operation:\n");
                printf("1. New customer registration\n");
                printf("2. Show my profile\n");
                printf("3. Make a service request\n");
                printf("4. Show my requests\n");
                printf("5. Cancel a service request\n");
                printf("6. Exit\n");
                printf("-----------------------------------------------\n");
                printf("Enter your choice: ");
                scanf("%d", &m);
                if(m==1){
                    input_customer();
                }
                else if(m==2){ 
                    int c_id;
                    printf("Please enter your customer Id: ");
                    scanf("%d", &c_id);
                    display_customer(c_id);
                }
                else if(m==3){
                    input_service_request();
                } 
                else if(m==4){
                    int c_id;
                    printf("Please enter your customer Id: ");
                    scanf("%d", &c_id);
                    display_requests_of_customer(c_id);
                }
                else if(m==5){
                    printf("PLease enter your customer Id: ");
                    int c_id;
                    scanf("%d", &c_id);
                    int r_id;
                    printf("Please enter the request Id: ");
                    scanf("%d", &r_id);
                    Service_request* req = findServiceRequest(r_id);
                    if(req==NULL){
                        printf("Request Id %d does not exist, cannot cancel the request\n", r_id);
                    }
                    else if(req->customer->id!=c_id){ 
                        printf("Request Id %d does not belong to customer Id %d, cannot cancel the request\n", r_id, c_id);
                    }
                    else{
                        cancel_service_request(r_id);
                    }
                }
                else if(m!=6){
                    printf("Invalid choice. Please try again.\n");
                }
            }while(m!=6);
        }
        else if(n==2){
            int m;
            do{
                printf("-----------------------------------------------\n");
                printf("Please select the operation:\n");
                printf("1. Register as a new professional\n");
                printf("2. Show my service history\n");
                printf("3. Exit\n");
                printf("-----------------------------------------------\n");
                printf("Enter your choice: ");
                scanf("%d", &m);
                if(m==1){
                    input_professional();
                }
                else if(m==2){
                    int p_id;
                    printf("Please enter your professional Id: ");
                    scanf("%d", &p_id);
                    professional_service_history(p_id);
                }
                else if(m!=3){
                    printf("Invalid choice. Please try again.\n");
                }
            }while(m!=3);
        }
        else if(n==3){
            int m;
            do{
                printf("-----------------------------------------------\n");
                printf("Please select the operation:\n");
                printf("1. Add a new service\n");
                printf("2. Complete a service request\n");
                printf("3. Display available professionals providing a service\n");
                printf("4. Display services provided by a professional\n");
                printf("5. Display requests made by a customer\n");
                printf("6. Display services based on demands\n");
                printf("7. Display professional activity\n");
                printf("8. Display all pending requests\n");
                printf("9. Display the service history of a professional\n");
                printf("10. Show all exisiting services\n");
                printf("11. Show all exisiting professionals\n");
                printf("12. Show all exisiting customers\n");
                printf("13. Range search requests by customer IDs\n");
                printf("14. Exit\n");
                printf("-----------------------------------------------\n");
                printf("Enter your choice: ");
                scanf("%d", &m);
                if(m==1){
                    input_service();
                }
                else if(m==2){
                    int r_id;
                    printf("Please enter the request Id: ");
                    scanf("%d", &r_id);
                    complete_assigned_service(r_id);
                }
                else if(m==3){
                    int s_id;
                    printf("Please enter the service Id: ");
                    scanf("%d", &s_id);
                    display_professionals_with_service(s_id);
                }
                else if(m==4){
                    int p_id;
                    printf("Please enter the professional Id: ");
                    scanf("%d", &p_id);
                    display_services_of_professional(p_id);
                }
                else if(m==5){
                    int c_id;
                    printf("Please enter the customer Id: ");
                    scanf("%d", &c_id);
                    display_requests_of_customer(c_id);
                }
                else if(m==6){
                    display_service_demand();
                }
                else if(m==7){
                    display_professional_activity();
                }
                else if(m==8){
                    all_pending_requests();
                }
                else if(m==9){
                    int p_id;
                    printf("Please enter the professional Id: ");
                    scanf("%d", &p_id);
                    professional_service_history(p_id);
                }
                else if(m==10){
                    display_all_services();
                }
                else if(m==11){
                    display_all_professionals();
                }
                else if(m==12){
                    display_all_customers();
                }
                else if(m==13){
                    int c1, c2;
                    printf("Please enter the first customer Id: ");
                    scanf("%d", &c1);
                    printf("Please enter the second customer Id: ");
                    scanf("%d", &c2);
                    requests_in_range(c1, c2);
                }
                else if(m!=14){
                    printf("Invalid choice. Please try again.\n");
                }
            }while(m!=14);
        }
        else if(n!=4){
            printf("Invalid choice. Please try again.\n");
        }
    }while(n!=4);
    printf("---------------------------------------------------------------------\n");
    printf("Exiting the URBAN COMPANY SERVICE MANAGEMENT SYSTEM\n");
    printf("Thank you for using the URBAN COMPANY SERVICE MANAGEMENT SYSTEM\n");
    printf("Visit again\n");
    printf("---------------------------------------------------------------------\n");
    save_all_data();
    free_all_data();
}
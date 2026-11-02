#include "definitions.h"
#include "function_definitions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Customer *c_root = NULL;
Professional *p_root = NULL;
Service *s_root = NULL;
Service_request *r_root = NULL;

int next_cId = 1;
int next_pId = 1;
int next_sId = 1;
int next_rId = 1;

int c_height(Customer* root){
    int ret_val = 0;
    if(root!=NULL){
        ret_val = root->height;
    }
    return ret_val;
}

int p_height(Professional* root){
    int ret_val = 0;
    if(root!=NULL){
        ret_val = root->height;
    }
    return ret_val;
}

int s_height(Service* root){
    int ret_val = 0;
    if(root!=NULL){
        ret_val = root->height;
    }
    return ret_val;
}

int r_height(Service_request* root){
    int ret_val = 0;
    if(root!=NULL){
        ret_val = root->height;
    }
    return ret_val;
}

int max(int x, int y){
    int ret_val;
    if(x>y){
        ret_val = x;
    }
    else{
        ret_val = y;
    }
    return ret_val;
}

date_time get_current_date_time(){
    date_time dt = {0, 0, 0, 0, 0};
    time_t t = time(NULL);
    struct tm* now = localtime(&t);
    if(now!=NULL){
        dt.day = now->tm_mday;
        dt.month = now->tm_mon + 1;
        dt.year = now->tm_year + 1900;
        dt.hour = now->tm_hour;
        dt.minute = now->tm_min;
    }
    return dt;
}

void input_customer(){
    char name[NAME_LENGTH];
    int pincode;
    char contact[CONTACT_LENGTH];
    printf("Please enter your name: ");
    scanf(" %49[^\n]", name);  
    printf("Please enter your pincode: ");
    scanf("%d", &pincode);
    printf("Please enter your contact number: ");
    scanf(" %10[^\n]", contact);
    status_code result = registerCustomer(name, pincode, contact);
    if (result==FAILURE){
        printf("---------------------------------------------------\n");
        printf("Customer registration failed, Please try again\n");
    }
    save_all_data();
}

status_code registerCustomer(char name[], int pin, char contact[]){
    status_code sc = SUCCESS;
    if(find_duplicate_customer(name, pin, contact)!=NULL){
        printf("---------------------------------------------------\n");
        printf("Duplicate customer found with same name, pincode and contact number\n");
        sc = FAILURE;
        return sc;
    }
    Customer *nc = (Customer *) malloc(sizeof(Customer));
    if(nc == NULL){
        sc = FAILURE;
    }
    else{
        nc->id = next_cId;
        strcpy(nc->name, name);
        nc->pincode = pin;
        strcpy(nc->contact_number, contact);
        nc->height = 1;
        nc->left = NULL;
        nc->right = NULL;
        c_root = insert_c_tree(c_root, nc);
        printf("----------------------------------------\n");
        printf("Customer registered successfully \n");
        printf("Your customer Id is: %d\n", nc->id);
        printf("----------------------------------------\n");
        next_cId++;
    }
    return sc;
}

Customer* c_left_rotate(Customer* root){
    Customer* y = root->right;
    Customer* T2 = y->left;
    y->left = root;
    root->right = T2;
    root->height = 1 + max(c_height(root->left), c_height(root->right));
    y->height = 1 + max(c_height(y->left), c_height(y->right));
    return y;
}

Customer* c_right_rotate(Customer* root){
    Customer* x = root->left;
    Customer* T2 = x->right;
    x->right = root;
    root->left = T2;
    root->height = 1 + max(c_height(root->left), c_height(root->right));
    x->height = 1 + max(c_height(x->left), c_height(x->right));
    return x;
}

Customer* insert_c_tree(Customer* root, Customer* nc){
    Customer* ret_val = NULL;
    if(root==NULL){
        ret_val = nc;
    }
    else{
        if(nc->id>root->id){
            root->right = insert_c_tree(root->right,nc);
        }
        else if(nc->id<root->id){
            root->left = insert_c_tree(root->left, nc);
        }

        root->height = 1 + max(c_height(root->left), c_height(root->right));
        int bf = (c_height(root->left)) - (c_height(root->right));
        if(bf>1 && nc->id<root->left->id){
            ret_val = c_right_rotate(root);
        }
        else if(bf<-1 && nc->id>root->right->id){
            ret_val = c_left_rotate(root);
        }
        else if(bf>1 && nc->id>root->left->id){
            root->left = c_left_rotate(root->left);
            ret_val = c_right_rotate(root);
        }
        else if(bf<-1 && nc->id<root->right->id){
            root->right = c_right_rotate(root->right);
            ret_val = c_left_rotate(root);
        }
        else{
            ret_val = root;
        }
    }
    return ret_val;
}

void input_professional(){
    char name[NAME_LENGTH];
    int pincode;
    char contact[CONTACT_LENGTH];
    Service* services[MAX_SERVICES];
    for(int i = 0; i<MAX_SERVICES; i++){
        services[i] = NULL;
    }
    printf("Please enter your name: ");
    scanf(" %49[^\n]", name);
    printf("Please enter your pincode: ");
    scanf("%d", &pincode);
    printf("Please enter your contact number: ");
    scanf(" %10[^\n]", contact);
    int total_s = 0;
    display_all_services();
    do{
        printf("Please enter the number of services you provide(you can provide max %d): ", MAX_SERVICES);
        scanf("%d", &total_s);
        if(total_s<1 || total_s>MAX_SERVICES){
            printf("Invalid number of services, please try again\n"); 
        }
    } 
    while(total_s<1 || total_s>MAX_SERVICES);

    int index = 0;
    if(total_s>next_sId-1){
        total_s = next_sId-1;
    }
    while(index<total_s){
        int s_id;
        printf("Please enter the service Id for service %d: ", index+1);
        scanf("%d", &s_id);
        int flag = 0;
        for(int j = 0; j<index; j++){ 
            if(services[j]!=NULL && services[j]->id==s_id){
                printf("Duplicate service Id entered, please try again\n");
                flag = 1;
                index--;
                s_id = -1;
                break;
            }
        }
        if(flag==1){
            index++;
            continue;
        }
        Service* temp_service = findService(s_id);
        if(temp_service == NULL){
            printf("Service Id %d is invalid, this service will not be added.\n", s_id);
            index--;
        }
        else{
            services[index]=temp_service;
        }
        index++;
    }
    status_code check = registerProfessional(name, pincode, contact, services);
    if(check==FAILURE){
        printf("Professional registration failed. Please try again.\n");
    }
    else{
        Professional* np = findProfessional(next_pId-1);
        Service_request* new_req = new_request_for_professional(np);
        if(new_req == NULL){
            printf("No available request found for the professional.\n");
        }
        else{
            printf("New request assigned successfully for the professional.\n");
            printf("Request Id: %d\n", new_req->id);
            printf("Customer Id: %d\n", new_req->customer->id);
            printf("Service Id: %d\n", new_req->service->id);
            np->a_status = UNAVAILABLE;
            new_req->professional = np;
            new_req->status = ASSIGNED;
        }
    }
    save_all_data();
}

status_code registerProfessional(char name[], int pin, char contact[], Service *services[]){
    status_code sc = SUCCESS;
    if(find_duplicate_professional(name, pin, contact)!=NULL){
        printf("---------------------------------------------------\n");
        printf("Duplicate professional found with same name, pincode and contact number\n");
        sc = FAILURE;
        return sc;
    }
    Professional* np = (Professional*) malloc(sizeof(Professional));
    if (np==NULL) {
        sc = FAILURE;
    }
    else{
        np->id = next_pId;
        strcpy(np->name, name);
        np->pincode = pin;
        strcpy(np->contact_number, contact);
        np->a_status = AVAILABLE;
        for (int i = 0 ; i<MAX_SERVICES ; i++){
            np->services[i] = services[i];
        }
        np->height = 1;
        np->left = NULL;
        np->right = NULL;
        p_root = insert_p_tree(p_root, np);
        printf("---------------------------------------\n");
        printf("Professional registered successfully\n");
        printf("Your professional Id is: %d\n", np->id);
        printf("\n---------------------------------------\n");
        next_pId++;
    }
    return sc;
}

Professional* p_right_rotate(Professional* root){
    Professional* x = root->left;
    Professional* T2 = x->right;
    x->right = root;
    root->left = T2;
    root->height = 1 + max(p_height(root->left), p_height(root->right));
    x->height = 1 + max(p_height(x->left), p_height(x->right));
    return x;
}

Professional* p_left_rotate(Professional* root){
    Professional* y = root->right;
    Professional* T2 = y->left;
    y->left = root;
    root->right = T2;
    root->height = 1 + max(p_height(root->left), p_height(root->right));
    y->height = 1 + max(p_height(y->left), p_height(y->right));
    return y;
}

Professional* insert_p_tree(Professional* root, Professional* np){
    Professional* ret_val = NULL;
    if(root==NULL){
        ret_val = np;
    }
    else{
        if(np->id>root->id){
            root->right = insert_p_tree(root->right,np);
        }
        else if(np->id<root->id){
            root->left = insert_p_tree(root->left, np);
        }

        root->height = 1 + max(p_height(root->left), p_height(root->right));
        int bf = (p_height(root->left)) - (p_height(root->right));
        if(bf>1 && np->id<root->left->id){
            ret_val = p_right_rotate(root);
        }
        else if(bf<-1 && np->id>root->right->id){
            ret_val = p_left_rotate(root);
        }
        else if(bf>1 && np->id>root->left->id){
            root->left = p_left_rotate(root->left);
            ret_val = p_right_rotate(root);
        }
        else if(bf<-1 && np->id<root->right->id){
            root->right = p_right_rotate(root->right); 
            ret_val = p_left_rotate(root);
        }
        else{
            ret_val = root;
        }
    }
    return ret_val;
}

void input_service(){
    char name[NAME_LENGTH];
    int baseprice;
    printf("Please enter the service name: ");
    scanf(" %49[^\n]", name);
    printf("Please enter the base price for this service: ");
    scanf("%d", &baseprice);
    status_code result = addservice(name, baseprice);
    if (result==FAILURE){
        printf("Service addition failed. Please try again.\n");
    }
    save_all_data();
}

status_code addservice(char name[], int baseprice){
    status_code sc = SUCCESS;
    if(find_duplicate_service(name)!=NULL){
        printf("---------------------------------------------------\n");
        printf("Duplicate service found with same name\n");
        sc = FAILURE;
        return sc;
    }
    Service* ns = (Service*) malloc(sizeof(Service));
    if(ns==NULL){
        sc = FAILURE;
    }
    else{
        ns->id = next_sId;
        strcpy(ns->name, name);
        ns->baseprice = baseprice;
        ns->height = 1;
        ns->left = NULL;
        ns->right = NULL;
        s_root = insert_s_tree(s_root, ns);
        printf("---------------------------------------\n");
        printf("Service added successfully\n");
        printf("Your service Id is: %d", ns->id);
        printf("\n---------------------------------------\n");
        next_sId++;
    }
    return sc;
}

Service* s_right_rotate(Service* root){
    Service* x = root->left;
    Service* T2 = x->right;
    x->right = root;
    root->left = T2;
    root->height = 1 + max(s_height(root->left), s_height(root->right));
    x->height = 1 + max(s_height(x->left), s_height(x->right));
    return x;
}

Service* s_left_rotate(Service* root){
    Service* y = root->right;
    Service* T2 = y->left;
    y->left = root;
    root->right = T2;
    root->height = 1 + max(s_height(root->left), s_height(root->right));
    y->height = 1 + max(s_height(y->left), s_height(y->right));
    return y;
}

Service* insert_s_tree(Service* root, Service* ns){
    Service* ret_val = NULL;
    if(root==NULL){
        ret_val = ns;
    }
    else{
        if(ns->id>root->id){
            root->right = insert_s_tree(root->right,ns);
        }
        else if(ns->id<root->id){
            root->left = insert_s_tree(root->left, ns);
        }

        root->height = 1 + max(s_height(root->left), s_height(root->right));
        int bf = (s_height(root->left)) - (s_height(root->right));
        if(bf>1 && ns->id<root->left->id){
            ret_val = s_right_rotate(root);
        }
        else if(bf<-1 && ns->id>root->right->id){
            ret_val = s_left_rotate(root);
        }
        else if(bf>1 && ns->id>root->left->id){
            root->left = s_left_rotate(root->left);
            ret_val = s_right_rotate(root);
        }
        else if(bf<-1 && ns->id<root->right->id){
            root->right = s_right_rotate(root->right);
            ret_val = s_left_rotate(root);
        }
        else{
            ret_val = root;
        }
    }
    return ret_val;
}

void input_service_request(){
    int customer_id, service_id;
    date_time dt;
    printf("Please enter your customer Id: ");
    scanf("%d", &customer_id);
    display_all_services();
    printf("Please enter the service Id: ");
    scanf("%d", &service_id);
    dt = get_current_date_time();
    status_code result = createServiceRequest(customer_id, service_id, dt);
    if(result==FAILURE){
        printf("Service request creation failed. Please try again.\n");
    }
    save_all_data();
}

Customer* findCustomer(int id){
    Customer* ret_val = NULL;
    Customer* curr = c_root;
    while(curr!=NULL){
        if(id==curr->id){
            ret_val = curr;
            break;
        }
        else if(id<curr->id){
            curr = curr->left;
        }
        else{
            curr = curr->right;
        }
    }
    return ret_val;
}

Service* findService(int id){
    Service* ret_val = NULL;
    Service* curr = s_root;
    while(curr!=NULL){
        if(id==curr->id){
            ret_val = curr;
            break;
        }
        else if(id<curr->id){
            curr = curr->left;
        }
        else{
            curr = curr->right;
        }
    }
    return ret_val;
}

Professional* findProfessional(int id){
    Professional* ret_val = NULL;
    Professional* curr = p_root;
    while(curr!=NULL){
        if(id==curr->id){
            ret_val = curr;
            break;
        }
        else if(id<curr->id){
            curr = curr->left;
        }
        else{
            curr = curr->right;
        }
    }
    return ret_val;
}

Service_request* findServiceRequest(int id){
    Service_request* ret_val = NULL;
    Service_request* curr = r_root;
    while(curr!=NULL){
        if(id==curr->id){
            ret_val = curr;
            break;
        }
        else if(id<curr->id){
            curr = curr->left;
        }
        else{
            curr = curr->right;
        }
    }
    return ret_val;
}

status_code createServiceRequest(int c_id, int s_id, date_time dt){
    status_code sc = SUCCESS;
    Customer* c = findCustomer(c_id);
    Service* s = findService(s_id);
    if(c==NULL || s==NULL){
        sc = FAILURE;
        printf("Invalid customer Id or service Id\n");
    }
    else{
        Service_request* nr = (Service_request*)malloc(sizeof(Service_request));
        if(nr==NULL){
            sc = FAILURE;
        }
        else{
            nr->id = next_rId;
            nr->customer = c;
            nr->service = s;
            nr->professional = NULL;
            nr->dt = dt;
            nr->status = PENDING;
            nr->height = 1;
            nr->left = NULL;
            nr->right = NULL;
            r_root = insert_r_tree(r_root, nr);
            Professional* temp = check_for_professional(s_id, c->pincode);
            if(temp!=NULL){
                nr->professional = temp;
                nr->status = ASSIGNED;
                temp->a_status = UNAVAILABLE;
                printf("---------------------------------------\n");
                printf("The required professional is assigned to you\n");
                printf("Professional Id: %d\n", temp->id);
                printf("Professional Name: %s\n", temp->name);
                printf("Professional Contact Number: %s\n", temp->contact_number);
                printf("---------------------------------------\n");
            }
            else{
                printf("The required professional will be provided soon\n");
            }

            printf("---------------------------------------\n");
            printf("Service request created successfully\n");
            printf("Your request Id is: %d", nr->id); 
            printf("\n---------------------------------------\n");
            next_rId++;
        }
    }
    return sc;
}

Service_request* r_left_rotate(Service_request* root){
    Service_request* y = root->right;
    Service_request* T2 = y->left;
    y->left = root;
    root->right = T2;
    root->height = 1 + max(r_height(root->left), r_height(root->right));
    y->height = 1 + max(r_height(y->left), r_height(y->right));
    return y;
}

Service_request* r_right_rotate(Service_request* root){
    Service_request* x = root->left;
    Service_request* T2 = x->right;
    x->right = root;
    root->left = T2;
    root->height = 1 + max(r_height(root->left), r_height(root->right));
    x->height = 1 + max(r_height(x->left), r_height(x->right));
    return x;
}

Service_request* insert_r_tree(Service_request* root, Service_request* nr){
    Service_request* ret_val = NULL;
    if(root==NULL){
        ret_val = nr;
    }
    else{
        if(nr->id>root->id){
            root->right = insert_r_tree(root->right,nr);
        }
        else if(nr->id<root->id){
            root->left = insert_r_tree(root->left, nr);
        }

        root->height = 1 + max(r_height(root->left), r_height(root->right));
        int bf = (r_height(root->left)) - (r_height(root->right));
        if(bf>1 && nr->id<root->left->id){
            ret_val = r_right_rotate(root);
        }
        else if(bf<-1 && nr->id>root->right->id){
            ret_val = r_left_rotate(root);
        }
        else if(bf>1 && nr->id>root->left->id){
            root->left = r_left_rotate(root->left);
            ret_val = r_right_rotate(root);
        }
        else if(bf<-1 && nr->id<root->right->id){
            root->right = r_right_rotate(root->right);
            ret_val = r_left_rotate(root);
        }
        else{
            ret_val = root;
        }
    }
    return ret_val;
}

void find_available_professional(Professional* root, int s_id, int pin, Professional** p1, Professional** p2){
    if(root!=NULL && *p2==NULL){
        find_available_professional(root->left, s_id, pin, p1, p2);
        if(*p2==NULL && root->a_status==AVAILABLE){
            int flag = 0;
            for(int i = 0; i<MAX_SERVICES && flag==0; i++){
                if(root->services[i]!=NULL && root->services[i]->id==s_id){
                    flag = 1;
                    if(*p1==NULL){
                        *p1 = root;
                    }
                    if(pin==root->pincode && *p2==NULL){
                        *p2 = root;
                    }
                }
            }
        }
        if(*p2==NULL){
            find_available_professional(root->right, s_id, pin, p1, p2);
        }
    }
    return;
}

Professional* check_for_professional(int s_id, int pin){
    Professional* p1 = NULL;
    Professional* p2 = NULL;
    find_available_professional(p_root, s_id, pin, &p1, &p2);
    if(p2!=NULL){
        return p2;
    }
    else{
        return p1;
    }
}

void find_job_for_professional(Service_request* root, Professional* p, Service_request** r1, Service_request** r2){
    if(root!=NULL && *r2==NULL){
        find_job_for_professional(root->left, p, r1, r2);
        for(int i = 0; i<MAX_SERVICES && *r2==NULL; i++){
            if(p->services[i]!=NULL && root->service->id==p->services[i]->id && root->status==PENDING){
                if(*r1==NULL){
                    *r1 = root;
                }
                if(*r2==NULL && root->customer->pincode==p->pincode){
                    *r2 = root;
                }
            }
        }
        if(*r2==NULL){
            find_job_for_professional(root->right, p, r1, r2);
        }
    }
    return;
}

Service_request* new_request_for_professional(Professional* p){
    Service_request* ret_val = NULL;
    Service_request* r1 = NULL;
    Service_request* r2 = NULL;
    find_job_for_professional(r_root, p, &r1, &r2);
    if(r2!=NULL){
        ret_val = r2;
    }
    else{
        ret_val = r1;
    }
    return ret_val;
}

void complete_assigned_service(int r_id){
    printf("----------------------------------------\n");
    Service_request* req = findServiceRequest(r_id);
    if(req==NULL){
        printf("The request Id is not correct\n");
    }
    else if(req->status==PENDING){
        printf("This service request is not assigned\n");
    }
    else if(req->status==COMPLETED){
        printf("This service request is already completed\n");
    }
    else if(req->status==CANCELLED){
        printf("This service request is cancelled\n");
    }
    else{
        req->status = COMPLETED;
        req->professional->a_status = AVAILABLE;
        printf("Service request completed successfully\n");
        Service_request* nr = new_request_for_professional(req->professional);
        if(nr!=NULL){
            nr->professional = req->professional;
            nr->status = ASSIGNED;
            req->professional->a_status = UNAVAILABLE;
            printf("The professional is assigned to a new service request\n");
            printf("Request Id: %d\n", nr->id);
            printf("Customer Name: %s\n", nr->customer->name);
            printf("Customer Contact Number: %s\n", nr->customer->contact_number);
        }
        else{
            printf("The professional is now available\n");
        }
    } 
    printf("----------------------------------------\n");
    save_all_data();
}

void cancel_service_request(int r_id){
    printf("----------------------------------------\n");
    Service_request* req = findServiceRequest(r_id);
    if(req==NULL){
        printf("The request Id is not correct\n");
    }
    else if(req->status==CANCELLED){
        printf("This service request is already cancelled\n");
    }
    else if(req->status==COMPLETED){
        printf("This service request is already completed\n");
    }
    else if(req->status==PENDING){
        req->status = CANCELLED;
        printf("Service request cancelled successfully\n");
    } 
    else{
        req->status = CANCELLED;
        req->professional->a_status = AVAILABLE;
        printf("Service request cancelled successfully\n");
        printf("-----------------------------------------\n");
        Service_request* nr = new_request_for_professional(req->professional);
        if(nr!=NULL){
            nr->professional = req->professional;
            nr->status = ASSIGNED;
            req->professional->a_status = UNAVAILABLE;
            printf("The professional is assigned to a new service request\n");
            printf("Request Id: %d\n", nr->id);
            printf("Customer Name: %s\n", nr->customer->name);
            printf("Customer Contact Number: %s\n", nr->customer->contact_number);
        }
        else{
            printf("The professional is now available\n");
        }
    }
    printf("----------------------------------------\n");
    save_all_data();
}

void traverse_professional_for_service(Professional* root, int s_id){
    if(root!=NULL){
        traverse_professional_for_service(root->left, s_id);
        for(int i = 0; i<MAX_SERVICES; i++){
            if(root->services[i]!=NULL && root->services[i]->id==s_id && root->a_status==AVAILABLE){
                printf("Professional Id: %d\n", root->id);
                printf("Professional Name: %s\n", root->name);
                printf("Professional Contact Number: %s\n", root->contact_number);
                printf("Professional Pincode: %d\n", root->pincode);
                printf("---------------------------------------\n");
            }
        }
        traverse_professional_for_service(root->right, s_id);
    }
}

void display_professionals_with_service(int s_id){
    printf("----------------------------------------\n");
    Service* s = findService(s_id);
    if(s==NULL){
        printf("The service Id is not correct\n");
    }
    else{
        printf("Professionals providing the service %s:\n", s->name);
        traverse_professional_for_service(p_root, s_id);
    }
    printf("----------------------------------------\n");
}

void display_services_of_professional(int p_id){
    printf("----------------------------------------\n");
    Professional* p = findProfessional(p_id);
    if(p==NULL){
        printf("The professional Id is not correct\n");
    }
    else{
        printf("Services provided by professional %s:\n", p->name);
        for(int i = 0; i<MAX_SERVICES; i++){
            if(p->services[i]!=NULL){
                printf("Service Id: %d\n", p->services[i]->id);
                printf("Service Name: %s\n", p->services[i]->name);
                printf("Service Base Price: %d\n", p->services[i]->baseprice);
                printf("---------------------------------------\n");
            }
        }
    }
}

void traverse_requests_of_customer(Service_request* root, int c_id){
    if(root!=NULL){
        traverse_requests_of_customer(root->left, c_id);
        if(root->customer->id==c_id){
            printf("Request Id: %d\n", root->id);
            printf("Service Name: %s\n", root->service->name); 
            printf("Service Base Price: %d\n", root->service->baseprice);
            printf("Request Date and Time: %d/%d/%d %02d:%02d\n", root->dt.day, root->dt.month, root->dt.year, root->dt.hour, root->dt.minute);
            printf("Request Status: ");
            if(root->status==PENDING){
                printf("PENDING\n");
            }
            else if(root->status==ASSIGNED){
                printf("ASSIGNED\n");
                printf("Professional Name: %s\n", root->professional->name);
                printf("Professional Contact Number: %s\n", root->professional->contact_number);
            }
            else if(root->status==COMPLETED){
                printf("COMPLETED\n");
                printf("Professional Name: %s\n", root->professional->name);
                printf("Professional Contact Number: %s\n", root->professional->contact_number);
            }
            else{
                printf("CANCELLED\n");
            }
            printf("---------------------------------------\n");
        }
        traverse_requests_of_customer(root->right, c_id);
    }
}
 
void display_requests_of_customer(int c_id){
    printf("----------------------------------------\n");
    Customer* c = findCustomer(c_id);
    if(c==NULL){
        printf("The customer Id is not correct\n");
    }
    else{
        printf("Service requests of customer %s:\n", c->name);
        traverse_requests_of_customer(r_root, c_id);
    }
    printf("----------------------------------------\n");
}

void display_requests_in_range(Service_request* root, int c_id1, int c_id2, int* flag){
    if(root!=NULL){
        display_requests_in_range(root->left, c_id1, c_id2, flag);
        if(root->customer->id>=c_id1 && root->customer->id<=c_id2){
            *flag = 1;
            printf("Request Id: %d\n", root->id);
            printf("Customer Name: %s\n", root->customer->name);
            printf("Service Name: %s\n", root->service->name); 
            printf("Service Base Price: %d\n", root->service->baseprice);
            printf("Request Date and Time: %d/%d/%d %02d:%02d\n", root->dt.day, root->dt.month, root->dt.year, root->dt.hour, root->dt.minute);
            printf("Request Status: ");
            if(root->status==PENDING){
                printf("PENDING\n");
            }
            else if(root->status==ASSIGNED){
                printf("ASSIGNED\n");
                printf("Professional Name: %s\n", root->professional->name);
                printf("Professional Contact Number: %s\n", root->professional->contact_number);
            }
            else if(root->status==COMPLETED){
                printf("COMPLETED\n");
                printf("Professional Name: %s\n", root->professional->name);
                printf("Professional Contact Number: %s\n", root->professional->contact_number);
            }
            else{
                printf("CANCELLED\n");
            }
            printf("---------------------------------------\n");
        }
        display_requests_in_range(root->right, c_id1, c_id2, flag);
    }
}

void requests_in_range(int c_id1, int c_id2){ 
    Customer* c1 = findCustomer(c_id1);
    Customer* c2 = findCustomer(c_id2);
    if(c1==NULL || c2==NULL){
        printf("Error: One or both of the customer Ids are not correct\n");
    }
    else if(c_id1>=c_id2){
        printf("Error: The first customer Id should be lesser than the second customer Id\n");
    }
    else{
        int flag = 0;
        printf("Service requests in the range of customer IDs %d to %d:\n", c_id1, c_id2);
        display_requests_in_range(r_root, c_id1, c_id2, &flag);
        if(flag==0){ 
            printf("No service requests found in the given range of customer IDs\n");
        }
    }
}

void display_pending_requests(Service_request* root, int* flag){
    if(root!=NULL){
        display_pending_requests(root->left, flag);
        if(root->status==PENDING){
            *flag = 1;
            printf("Request Id: %d\n", root->id);
            printf("Customer Name: %s\n", root->customer->name);
            printf("Service Name: %s\n", root->service->name); 
            printf("Service Base Price: %d\n", root->service->baseprice);
            printf("Request Date and Time: %d/%d/%d %02d:%02d\n", root->dt.day, root->dt.month, root->dt.year, root->dt.hour, root->dt.minute);
            printf("---------------------------------------\n");
        }
        display_pending_requests(root->right, flag);
    }
}

void all_pending_requests(){
    int flag = 0;
    printf("All pending service requests:\n");
    display_pending_requests(r_root, &flag);
    if(flag==0){
        printf("No pending service requests found\n");
    }
}

void display_service_history(Service_request* root, int p_id, int* flag){
    if(root!=NULL){
        display_service_history(root->left, p_id, flag);
        if(root->professional!=NULL && root->professional->id==p_id && root->status==COMPLETED){
            *flag = 1;
            printf("Request Id: %d\n", root->id);
            printf("Customer Name: %s\n", root->customer->name);
            printf("Service Name: %s\n", root->service->name); 
            printf("Service Base Price: %d\n", root->service->baseprice);
            printf("Request Date and Time: %d/%d/%d %02d:%02d\n", root->dt.day, root->dt.month, root->dt.year, root->dt.hour, root->dt.minute);
            printf("---------------------------------------\n");
        }
        display_service_history(root->right, p_id, flag);
    }
}

void professional_service_history(int p_id){
    Professional* p = findProfessional(p_id);
    if(p==NULL){
        printf("The professional Id is not correct\n");
    }
    else{
        int flag = 0;
        printf("Service history of professional %s:\n", p->name);
        display_service_history(r_root, p_id, &flag);
        if(flag==0){
            printf("No completed service requests found for this professional\n");
        }
    }
}

void service_demand_freq(Service_request* root, int* freq){
    if(root!=NULL){
        service_demand_freq(root->left, freq); 
        (freq[root->service->id-1])++;
        service_demand_freq(root->right, freq);
    }
}

void services_in_array(Service* root, Service** s_arr){
    if(root!=NULL){
        services_in_array(root->left, s_arr);
        s_arr[root->id-1] = root;
        services_in_array(root->right, s_arr);
    }
}

void display_service_demand(){
    if(next_sId==1){
        printf("There are no services available\n");
    }
    else{
        Service** s_arr = (Service**)malloc((next_sId-1)*sizeof(Service*));
        int* freq = (int*)malloc((next_sId-1)*sizeof(int));
        for(int i = 0; i<next_sId-1; i++){
            freq[i] = 0;
        }
        service_demand_freq(r_root, freq);
        services_in_array(s_root, s_arr);
        heapsort_services(s_arr, freq, next_sId-1);
        printf("\nThe following are the services in decreasing order of demand-->\n");
        for(int i = 0; i<next_sId-1; i++){
            printf("---------------------------------------------------\n");
            printf("Service Id: %d\n", s_arr[i]->id);
            printf("Service Name: %s\n", s_arr[i]->name);
            printf("Base Price: %d\n", s_arr[i]->baseprice);
            printf("Number of requests made for this service: %d\n", freq[i]); 
            printf("\n");
        }
        free(s_arr);
        free(freq);
    }
}

int service_comes_after(Service* a, int freq_a, Service* b, int freq_b){
    int ret_val = 0;
    if(freq_a<freq_b){
        ret_val = 1;
    }
    else if(freq_a==freq_b && a->id>b->id){
        ret_val = 1;
    }
    return ret_val;
}

void heapify_services(Service** s_arr, int* freq, int n, int i){
    int largest = i;
    int left = 2*i+1;
    int right = 2*i+2;
    if(left<n && service_comes_after(s_arr[left], freq[left], s_arr[largest], freq[largest])){
        largest = left;
    }
    if(right<n && service_comes_after(s_arr[right], freq[right], s_arr[largest], freq[largest])){
        largest = right;
    }
    if(largest!=i){
        Service* temp_s = s_arr[i];
        s_arr[i] = s_arr[largest];
        s_arr[largest] = temp_s;
        int temp_f = freq[i];
        freq[i] = freq[largest];
        freq[largest] = temp_f;
        heapify_services(s_arr, freq, n, largest);
    }
}

void heapsort_services(Service** s_arr, int* freq, int n){
    for(int i = n/2-1; i>=0; i--){
        heapify_services(s_arr, freq, n, i);
    }
    for(int i = n-1; i>0; i--){
        Service* temp_s = s_arr[0];
        s_arr[0] = s_arr[i];
        s_arr[i] = temp_s;
        int temp_f = freq[0];
        freq[0] = freq[i];
        freq[i] = temp_f;
        heapify_services(s_arr, freq, i, 0);
    }
}

void requests_professionals_freq(Service_request* root, int* freq){
    if(root!=NULL){
        requests_professionals_freq(root->left, freq); 
        if(root->professional!=NULL && root->status==COMPLETED){
            (freq[root->professional->id-1])++;
        }
        requests_professionals_freq(root->right, freq);
    }
}

void professionals_in_array(Professional* root, Professional** p_arr){
    if(root!=NULL){
        professionals_in_array(root->left, p_arr);
        p_arr[root->id-1] = root;
        professionals_in_array(root->right, p_arr);
    }
}

void display_professional_activity(){
    if(next_pId==1){
        printf("There are no professionals available\n");
    }
    else{
        Professional** p_arr = (Professional**)malloc((next_pId-1)*sizeof(Professional*));
        int* freq = (int*)malloc((next_pId-1)*sizeof(int));
        for(int i = 0; i<next_pId-1; i++){
            freq[i] = 0;
        }
        requests_professionals_freq(r_root, freq);
        professionals_in_array(p_root, p_arr);
        heapsort_professionals(p_arr, freq, next_pId-1);
        printf("\nThe following are the professionals in decreasing order of number of requests handled-->\n");
        for(int i = 0; i<next_pId-1; i++){
            printf("---------------------------------------------------\n");
            printf("Professional Id: %d\n", p_arr[i]->id);
            printf("Professional Name: %s\n", p_arr[i]->name);
            printf("Professional Contact Number: %s\n", p_arr[i]->contact_number);
            printf("Professional Pincode: %d\n", p_arr[i]->pincode);
            printf("Number of requests handled by this professional: %d\n", freq[i]); 
            printf("\n");
        }
        free(p_arr);
        free(freq);
    }
}

int professional_comes_after(Professional* a, int freq_a, Professional* b, int freq_b){
    int ret_val = 0;
    if(freq_a<freq_b){
        ret_val = 1;
    }
    else if(freq_a==freq_b && a->id>b->id){
        ret_val = 1;
    }
    return ret_val;
}

void heapify_professionals(Professional** p_arr, int* freq, int n, int i){
    int largest = i;
    int left = 2*i+1;
    int right = 2*i+2;
    if(left<n && professional_comes_after(p_arr[left], freq[left], p_arr[largest], freq[largest])){
        largest = left;
    }
    if(right<n && professional_comes_after(p_arr[right], freq[right], p_arr[largest], freq[largest])){
        largest = right;
    }
    if(largest!=i){
        Professional* temp_p = p_arr[i];
        p_arr[i] = p_arr[largest];
        p_arr[largest] = temp_p;
        int temp_f = freq[i];
        freq[i] = freq[largest];
        freq[largest] = temp_f;
        heapify_professionals(p_arr, freq, n, largest);
    }
}

void heapsort_professionals(Professional** p_arr, int* freq, int n){
    for(int i = n/2-1; i>=0; i--){
        heapify_professionals(p_arr, freq, n, i);
    }
    for(int i = n-1; i>0; i--){
        Professional* temp_p = p_arr[0];
        p_arr[0] = p_arr[i];
        p_arr[i] = temp_p;
        int temp_f = freq[0];
        freq[0] = freq[i];
        freq[i] = temp_f;
        heapify_professionals(p_arr, freq, i, 0);
    }
}

void traverse_services(Service* root){
    if(root!=NULL){
        traverse_services(root->left);
        printf("Service Id: %d\n", root->id);
        printf("Service Name: %s\n", root->name);
        printf("Base Price: %d\n", root->baseprice);
        printf("---------------------------------------\n");
        traverse_services(root->right);
    }
}

void display_all_services(){
    printf("----------------------------------------\n");
    if(next_sId==1){
        printf("There are no services available\n");
    }
    else{
        printf("The following are the services available-->\n");
        traverse_services(s_root);
    }
    printf("----------------------------------------\n");
}

void display_customer(int c_id){
    printf("----------------------------------------\n");
    Customer* c = findCustomer(c_id);
    if(c==NULL){
        printf("The customer Id is not correct\n");
    }
    else{
        printf("Customer Id: %d\n", c->id);
        printf("Customer Name: %s\n", c->name);
        printf("Customer Pincode: %d\n", c->pincode);
        printf("Customer Contact Number: %s\n", c->contact_number);
    }
    printf("---------------------------------------\n");
}

Customer* search_duplicate_customer(Customer* root, char name[], int pin, char contact[]){
    Customer* ret_val = NULL;
    if(root!=NULL){
        ret_val = search_duplicate_customer(root->left, name, pin, contact);
        if(ret_val==NULL && strcmp(root->name, name)==0 && root->pincode==pin && strcmp(root->contact_number, contact)==0){
            ret_val = root;
            return ret_val;
        }
        if(ret_val==NULL){
            ret_val = search_duplicate_customer(root->right, name, pin, contact);
        }
    }
    return ret_val;
}

Customer* find_duplicate_customer(char name[], int pin, char contact[]){
    Customer* ret_val = search_duplicate_customer(c_root, name, pin, contact);
    return ret_val;
}

Professional* search_duplicate_professional(Professional* root, char name[], int pin, char contact[]){
    Professional* ret_val = NULL;
    if(root!=NULL){
        ret_val = search_duplicate_professional(root->left, name, pin, contact);
        if(ret_val==NULL && strcmp(root->name, name)==0 && root->pincode==pin && strcmp(root->contact_number, contact)==0){
            ret_val = root;
            return ret_val;
        }
        if(ret_val==NULL){
            ret_val = search_duplicate_professional(root->right, name, pin, contact);
        }
    }
    return ret_val;
}

Professional* find_duplicate_professional(char name[], int pin, char contact[]){
    Professional* ret_val = search_duplicate_professional(p_root, name, pin, contact);
    return ret_val;
}

Service* search_duplicate_service(Service* root, char name[]){
    Service* ret_val = NULL;
    if(root!=NULL){
        ret_val = search_duplicate_service(root->left, name);
        if(ret_val==NULL && strcmp(root->name, name)==0){
            ret_val = root;
            return ret_val;
        }
        if(ret_val==NULL){
            ret_val = search_duplicate_service(root->right, name);
        }
    }
    return ret_val;
}

Service* find_duplicate_service(char name[]){
    Service* ret_val = search_duplicate_service(s_root, name);
    return ret_val;
}

void traverse_customers(Customer* root){
    if(root!=NULL){
        traverse_customers(root->left);
        printf("Customer ID : %d\n", root->id);
        printf("Customer Name  : %s\n", root->name);
        printf("Customer Pincode : %d\n", root->pincode);
        printf("Customer Contact Number : %s\n", root->contact_number);
        printf("---------------------------------------\n");
        traverse_customers(root->right);
    }
}

void display_all_customers(){
    if(next_cId==1){
        printf("No customers are registered\n");
    }
    else{
        traverse_customers(c_root);
    }
}

void traverse_professionals(Professional* root){
    if(root!=NULL){
        traverse_professionals(root->left);
        printf("Professional ID : %d\n", root->id);
        printf("Professional Name  : %s\n", root->name);
        printf("Professional Pincode : %d\n", root->pincode);
        printf("Professional Contact Number : %s\n", root->contact_number);
        printf("---------------------------------------\n");
        traverse_professionals(root->right);
    }
}

void display_all_professionals(){
    if(next_pId==1){
        printf("No professionals are registered\n");
    }
    else{
        traverse_professionals(p_root);
    }
}

void free_c_tree(Customer* root){
    if(root!=NULL){
        free_c_tree(root->left);
        free_c_tree(root->right);
        free(root);
    }
}

void free_p_tree(Professional* root){
    if(root!=NULL){
        free_p_tree(root->left);
        free_p_tree(root->right);
        free(root);
    }
}

void free_s_tree(Service* root){
    if(root!=NULL){
        free_s_tree(root->left);
        free_s_tree(root->right);
        free(root);
    }
}

void free_r_tree(Service_request* root){
    if(root!=NULL){
        free_r_tree(root->left);
        free_r_tree(root->right);
        free(root);
    }
}

void free_all_data(){
    free_c_tree(c_root);
    c_root = NULL;
    free_p_tree(p_root);
    p_root = NULL;
    free_s_tree(s_root);
    s_root = NULL;
    free_r_tree(r_root);
    r_root = NULL;
}

//--------------------------------------------------------------------------------------------------------

void save_s_tree(Service* root, FILE* file){
    if(root!=NULL){
        save_s_tree(root->left, file);
        fprintf(file, "%d|%s|%d\n", root->id, root->name, root->baseprice);
        save_s_tree(root->right, file);
    }
}

void save_c_tree(Customer* root, FILE* file){
    if(root!=NULL){
        save_c_tree(root->left, file);
        fprintf(file, "%d|%s|%d|%s\n", root->id, root->name, root->pincode, root->contact_number);
        save_c_tree(root->right, file);
    }
}

void save_p_tree(Professional* root, FILE* file){
    if(root!=NULL){
        save_p_tree(root->left, file);
        fprintf(file, "%d|%s|%d|%s|%d", root->id, root->name, root->pincode, root->contact_number, (int)root->a_status);
        for(int i = 0; i<MAX_SERVICES; i++){
            if(root->services[i]!=NULL){
                fprintf(file, "|%d", root->services[i]->id);
            }
            else{
                fprintf(file, "|0");
            }
        }
        fprintf(file, "\n");
        save_p_tree(root->right, file);
    }
}

void save_r_tree(Service_request* root, FILE* file){
    if(root!=NULL){
        save_r_tree(root->left, file);
        int p_id = 0;
        if(root->professional!=NULL){
            p_id = root->professional->id;
        }
        fprintf(file, "%d|%d|%d|%d|%d|%d|%d|%d|%d|%d\n", root->id, root->customer->id, root->service->id, p_id, root->dt.day, root->dt.month, root->dt.year, root->dt.hour, root->dt.minute, (int)root->status);
        save_r_tree(root->right, file);
    }
}

void save_all_data(){
    FILE* file = fopen("services.txt", "w");
    if(file==NULL){
        printf("Could not open services.txt for writing\n");
    }
    else{
        save_s_tree(s_root, file);
        fclose(file);
    }

    file = fopen("customers.txt", "w");
    if(file==NULL){
        printf("Could not open customers.txt for writing\n");
    }
    else{
        save_c_tree(c_root, file);
        fclose(file);
    }

    file = fopen("professionals.txt", "w");
    if(file==NULL){
        printf("Could not open professionals.txt for writing\n");
    }
    else{
        save_p_tree(p_root, file);
        fclose(file);
    }

    file = fopen("requests.txt", "w");
    if(file==NULL){
        printf("Could not open requests.txt for writing\n");
    }
    else{
        save_r_tree(r_root, file);
        fclose(file);
    }
}

void load_services(){
    FILE* file = fopen("services.txt", "r");
    if(file!=NULL){
        char line[256];
        char name[NAME_LENGTH];
        int id;
        int baseprice;
        next_line(file, line);
        while(line[0]!='\0'){
            if(sscanf(line, "%d|%49[^|]|%d", &id, name, &baseprice)==3){
                if(findService(id)!=NULL){
                    printf("Service Id %d is repeated in services.txt, skipping it\n", id);
                }
                else{
                    Service* ns = (Service*) malloc(sizeof(Service));
                    if(ns!=NULL){
                        ns->id = id;
                        strcpy(ns->name, name);
                        ns->baseprice = baseprice;
                        ns->height = 1;
                        ns->left = NULL;
                        ns->right = NULL;
                        s_root = insert_s_tree(s_root, ns);
                        if(id>=next_sId){
                            next_sId = id+1;
                        }
                    }
                }
            }
            next_line(file, line);
        }
        fclose(file);
    }
}

void load_customers(){
    FILE* file = fopen("customers.txt", "r");
    if(file!=NULL){
        char line[256];
        char name[NAME_LENGTH];
        char contact[CONTACT_LENGTH];
        int id;
        int pincode;
        next_line(file, line);
        while(line[0]!='\0'){
            if(sscanf(line, "%d|%49[^|]|%d|%10[^|]", &id, name, &pincode, contact)==4){
                if(findCustomer(id)!=NULL){
                    printf("Customer Id %d is repeated in customers.txt, skipping it\n", id);
                }
                else{
                    Customer* nc = (Customer*) malloc(sizeof(Customer));
                    if(nc!=NULL){
                        nc->id = id;
                        strcpy(nc->name, name);
                        nc->pincode = pincode;
                        strcpy(nc->contact_number, contact);
                        nc->height = 1;
                        nc->left = NULL;
                        nc->right = NULL;
                        c_root = insert_c_tree(c_root, nc);
                        if(id>=next_cId){
                            next_cId = id+1;
                        }
                    }
                }
            }
            next_line(file, line);
        }
        fclose(file);
    }
}

void load_professionals(){
    FILE* file = fopen("professionals.txt", "r");
    if(file!=NULL){
        char line[256];
        char name[NAME_LENGTH];
        char contact[CONTACT_LENGTH];
        int id;
        int pincode;
        int status;
        int s_ids[MAX_SERVICES];
        next_line(file, line);
        while(line[0]!='\0'){
            if(sscanf(line, "%d|%49[^|]|%d|%10[^|]|%d|%d|%d|%d|%d|%d", &id, name, &pincode, contact, &status, &s_ids[0], &s_ids[1], &s_ids[2], &s_ids[3], &s_ids[4])==10){
                if(findProfessional(id)!=NULL){
                    printf("Professional Id %d is repeated in professionals.txt, skipping it\n", id);
                }
                else{
                    Professional* np = (Professional*) malloc(sizeof(Professional));
                    if(np!=NULL){
                        np->id = id;
                        strcpy(np->name, name);
                        np->pincode = pincode;
                        strcpy(np->contact_number, contact);
                        np->a_status = (availability)status;
                        for(int i = 0; i<MAX_SERVICES; i++){
                            if(s_ids[i]==0){
                                np->services[i] = NULL;
                            }
                            else{
                                np->services[i] = findService(s_ids[i]);
                            }
                        }
                        np->height = 1;
                        np->left = NULL;
                        np->right = NULL;
                        p_root = insert_p_tree(p_root, np);
                        if(id>=next_pId){
                            next_pId = id+1;
                        }
                    }
                }
            }
            next_line(file, line);
        }
        fclose(file);
    }
}

void load_requests(){
    FILE* file = fopen("requests.txt", "r");
    if(file!=NULL){
        char line[256];
        int id;
        int c_id;
        int s_id;
        int p_id;
        int status;
        date_time dt;
        next_line(file, line);
        while(line[0]!='\0'){
            if(sscanf(line, "%d|%d|%d|%d|%d|%d|%d|%d|%d|%d", &id, &c_id, &s_id, &p_id, &dt.day, &dt.month, &dt.year, &dt.hour, &dt.minute, &status)==10){
                Customer* c = findCustomer(c_id);
                Service* s = findService(s_id);
                Professional* p = NULL;
                if(p_id!=0){
                    p = findProfessional(p_id);
                }
                if(findServiceRequest(id)!=NULL){
                    printf("Request Id %d is repeated in requests.txt, skipping it\n", id);
                }
                else if(c==NULL || s==NULL || (p_id!=0 && p==NULL) || ((status==ASSIGNED || status==COMPLETED) && p==NULL)){
                    printf("Request Id %d has invalid references in requests.txt, skipping it\n", id);
                }
                else{
                    Service_request* nr = (Service_request*) malloc(sizeof(Service_request));
                    if(nr!=NULL){
                        nr->id = id;
                        nr->customer = c;
                        nr->service = s;
                        nr->professional = p;
                        nr->dt = dt;
                        nr->status = (request_status)status;
                        nr->height = 1;
                        nr->left = NULL;
                        nr->right = NULL;
                        r_root = insert_r_tree(r_root, nr);
                        if(id>=next_rId){
                            next_rId = id+1;
                        }
                    }
                }
            }
            next_line(file, line);
        }
        fclose(file);
    }
}

status_code load_all_data(){
    status_code ret_val = SUCCESS;
    FILE* file = fopen("services.txt", "r");
    if(file==NULL){
        ret_val = FAILURE;
    }
    else{
        fclose(file);
        load_services();
        load_customers();
        load_professionals();
        load_requests();
    }
    return ret_val;
}



void merge_services(Service** s_arr, int* freq, int start, int mid, int end, Service** temp1, int* temp2){
    int i = start;
    int j = mid+1;
    int k = start;
    while(i<=mid && j<=end){
        if(freq[i]>=freq[j]){
            temp1[k] = s_arr[i];
            temp2[k] = freq[i];
            i++;
        }
        else{
            temp1[k] = s_arr[j];
            temp2[k] = freq[j];
            j++;
        }
        k++;
    }
    while(i<=mid){
        temp1[k] = s_arr[i];
        temp2[k] = freq[i];
        i++;
        k++;
    }
    while(j<=end){
        temp1[k] = s_arr[j];
        temp2[k] = freq[j];
        j++;
        k++;
    }
    for(i = start; i<=end; i++){
        s_arr[i] = temp1[i];
        freq[i] = temp2[i];
    }
}

void mergesort_services(Service** s_arr, int* freq, int start, int end, Service** temp1, int* temp2){
    if(start < end){
        int mid = start + (end-start)/2;
        mergesort_services(s_arr, freq, start, mid, temp1, temp2);
        mergesort_services(s_arr, freq, mid+1, end, temp1, temp2);
        merge_services(s_arr, freq, start, mid, end, temp1, temp2);
    }
}

void merge_professionals(Professional** p_arr, int* freq, int start, int mid, int end, Professional** temp1, int* temp2){
    int i = start;
    int j = mid+1;
    int k = start;
    while(i<=mid && j<=end){
        if(freq[i]>=freq[j]){
            temp1[k] = p_arr[i];
            temp2[k] = freq[i];
            i++;
        }
        else{
            temp1[k] = p_arr[j];
            temp2[k] = freq[j];
            j++;
        }
        k++;
    }
    while(i<=mid){
        temp1[k] = p_arr[i];
        temp2[k] = freq[i];
        i++;
        k++;
    }
    while(j<=end){
        temp1[k] = p_arr[j];
        temp2[k] = freq[j];
        j++;
        k++;
    }
    for(i = start; i<=end; i++){
        p_arr[i] = temp1[i];
        freq[i] = temp2[i];
    }
}

void mergesort_professionals(Professional** p_arr, int* freq, int start, int end, Professional** temp1, int* temp2){
    if(start < end){
        int mid = start + (end-start)/2;
        mergesort_professionals(p_arr, freq, start, mid, temp1, temp2);
        mergesort_professionals(p_arr, freq, mid+1, end, temp1, temp2);
        merge_professionals(p_arr, freq, start, mid, end, temp1, temp2);
    }
}

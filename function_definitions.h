#ifndef FUNCTION_DEFINITIONS_H
#define FUNCTION_DEFINITIONS_H
#include "definitions.h"

int c_height(Customer* root);

int p_height(Professional* root);

int s_height(Service* root);

int r_height(Service_request* root);

int max(int x, int y);

date_time get_current_date_time();

void input_customer();

status_code registerCustomer(char name[], int pin, char contact[]);

Customer* c_left_rotate(Customer* root);

Customer* c_right_rotate(Customer* root);

Customer* insert_c_tree(Customer* root, Customer* nc);

void input_professional();

status_code registerProfessional(char name[], int pin, char contact[], Service* services[]);

Professional* p_right_rotate(Professional* root);

Professional* p_left_rotate(Professional* root);

Professional* insert_p_tree(Professional* root, Professional* np);

void input_service();

status_code addservice(char name[], int baseprice);

Service* s_right_rotate(Service* root);

Service* s_left_rotate(Service* root);

Service* insert_s_tree(Service* root, Service* ns);

void input_service_request();

Customer* findCustomer(int id);

Service* findService(int id);

Professional* findProfessional(int id);

Service_request* findServiceRequest(int id);

status_code createServiceRequest(int c_id, int s_id, date_time dt);

Service_request* r_left_rotate(Service_request* root);

Service_request* r_right_rotate(Service_request* root);

Service_request* insert_r_tree(Service_request* root, Service_request* nr);

void find_available_professional(Professional* root, int s_id, int pin, Professional** p1, Professional** p2);

Professional* check_for_professional(int s_id, int pin);

void find_job_for_professional(Service_request* root, Professional* p, Service_request** r1, Service_request** r2);

Service_request* new_request_for_professional(Professional* p);

void complete_assigned_service(int r_id);

void cancel_service_request(int r_id);

void traverse_professional_for_service(Professional* root, int s_id);

void display_professionals_with_service(int s_id);

void display_services_of_professional(int p_id);

void traverse_requests_of_customer(Service_request* root, int c_id);

void display_requests_of_customer(int c_id);

void display_requests_in_range(Service_request* root, int c_id1, int c_id2, int* flag);

void requests_in_range(int c_id1, int c_id2);

void display_pending_requests(Service_request* root, int* flag);

void all_pending_requests();

void display_service_history(Service_request* root, int p_id, int* flag);

void professional_service_history(int p_id);

void service_demand_freq(Service_request* root, int* freq);

void services_in_array(Service* root, Service** s_arr);

void display_service_demand();

void requests_professionals_freq(Service_request* root, int* freq);

void professionals_in_array(Professional* root, Professional** p_arr);

void display_professional_activity();

void heapify_services(Service** s_arr, int* freq, int n, int i);

void heapsort_services(Service** s_arr, int* freq, int n);

void heapify_professionals(Professional** p_arr, int* freq, int n, int i);

void heapsort_professionals(Professional** p_arr, int* freq, int n);

int professional_comes_after(Professional* p1, int f1, Professional* p2, int f2);

int service_comes_after(Service* s1, int f1, Service* s2, int f2);

void traverse_services(Service* root);

void display_all_services();

void display_customer(int c_id);

Customer* find_duplicate_customer(char name[], int pin, char contact[]);

Customer* search_duplicate_customer(Customer* root, char name[], int pin, char contact[]);

Professional* find_duplicate_professional(char name[], int pin, char contact[]);

Professional* search_duplicate_professional(Professional* root, char name[], int pin, char contact[]);

Service* find_duplicate_service(char name[]);

Service* search_duplicate_service(Service* root, char name[]);

void read_hardcoded_data();

void next_line(FILE* file, char line[]);

void display_all_customers();

void display_all_professionals();

void traverse_professionals(Professional* root);

void traverse_customers(Customer* root);

void free_c_tree(Customer* root);

void free_p_tree(Professional* root);

void free_s_tree(Service* root);

void free_r_tree(Service_request* root);

void free_all_data();

void save_s_tree(Service* root, FILE* file);

void save_c_tree(Customer* root, FILE* file);

void save_p_tree(Professional* root, FILE* file);

void save_r_tree(Service_request* root, FILE* file);

void save_all_data();

void load_services();

void load_customers();

void load_professionals();

void load_requests();

status_code load_all_data();

#endif
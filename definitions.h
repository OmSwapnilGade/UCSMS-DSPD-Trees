#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#include<stdio.h>
#include<stdlib.h>

#define NAME_LENGTH 50
#define MAX_SERVICES 5
#define CONTACT_LENGTH 11

typedef enum {UNAVAILABLE, AVAILABLE} availability;
typedef enum {PENDING, ASSIGNED, COMPLETED, CANCELLED} request_status;
typedef enum {SUCCESS, FAILURE} status_code;

typedef struct date_time_tag{
    int day;
    int month;
    int year;
    int hour;
    int minute;
} date_time;

typedef struct customer_tag{
    int id;
    char name[NAME_LENGTH];
    int pincode;
    char contact_number[CONTACT_LENGTH];
    int height;
    struct customer_tag *left;
    struct customer_tag *right;
} Customer;

typedef struct service_tag{
    int id;
    char name[NAME_LENGTH];
    int baseprice;
    int height;
    struct service_tag *left;
    struct service_tag *right;
} Service;

typedef struct professional_tag{
    int id;
    char name[NAME_LENGTH];
    int pincode;
    char contact_number[CONTACT_LENGTH];
    availability a_status;
    Service* services[MAX_SERVICES];
    int height;
    struct professional_tag *left;
    struct professional_tag *right;
} Professional;

typedef struct service_request_tag{
    int id;
    Customer* customer;
    Service* service;
    Professional* professional;
    date_time dt;
    request_status status;
    int height;
    struct service_request_tag *left;
    struct service_request_tag *right;
} Service_request;


#endif
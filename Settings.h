#ifndef SETTINGS_H
#define SETTINGS_H
#pragma once
#define EPSILON 0.0001

#define INDEX_OFFSET_ 25 // Number of specs that have been done
#define NUM_OF_ORGANIC_MATERIAL 33
#define START_WL 300
#define END_WL 1000
#define JUMP_WL_need_upgrade 0

#define ERR_SUCCESS 0 // Operation successful
#define ERR_UNKNOWN -1 // Unknown error
#define ERR_INVALID_ARGUMENT -2 // Invalid argument
#define ERR_OUT_OF_MEMORY -3 // Out of memory
#define ERR_FILE -4 // File problems
#define ERR_IO_ERROR -5 // Input/output error

#define ERR_DB_CONNECTION_FAILED -100 // Database connection failed
#define ERR_DB_QUERY_FAILED -101 // Database query failed
#define ERR_DB_RECORD_NOT_FOUND -102 // Database record not found

#define ORGINAL_FILE_PATH "./orginal"
#define DATA_FILE_PATH "./data"
#define DATABASE_PATH "./database"

#endif // SETTINGS_H
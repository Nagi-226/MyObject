// database.c
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

sqlite3 *db;

int init_db(const char *db_name) {
    if (sqlite3_open(db_name, &db)) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    return 1;
}

void close_db() {
    sqlite3_close(db);
}

int register_user(const char *username, const char *password) {
    char *err_msg = 0;
    char sql[256];
    snprintf(sql, sizeof(sql), "INSERT INTO users (username, password) VALUES ('%s', '%s');", username, password);
    if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 0;
    }
    return 1;
}

int authenticate_user(const char *username, const char *password) {
    sqlite3_stmt *stmt;
    char sql[256];
    snprintf(sql, sizeof(sql), "SELECT * FROM users WHERE username='%s' AND password='%s';", username, password);
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    int res = sqlite3_step(stmt) == SQLITE_ROW;
    sqlite3_finalize(stmt);
    return res;
}

int log_query(int user_id, const char *word) {
    char *err_msg = 0;
    char sql[256];
    snprintf(sql, sizeof(sql), "INSERT INTO queries (user_id, word) VALUES (%d, '%s');", user_id, word);
    if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 0;
    }
    return 1;
}
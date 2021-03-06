#include "db.hpp"

#include <stdexcept>
#include <iostream>
#include "logger.hpp"

using namespace std;

#define DB_NAME "unipic.db"

/**
 * global db instance
 */
sqlite3 *db = nullptr;

/**
 * sql to create table
 */
const char *createSQL = 
"CREATE TABLE IF NOT EXISTS image_storage ("
  "pixels INT NOT NULL,"
  "fingerprint TEXT UNIQUE NOT NULL,"
  "path TEXT UNIQUE NOT NULL"
");"
;

/**
 * type of sqlite3_exec callback
 */
typedef int(*sqlite_cb)(void*, int, char**, char**);

/**
 * exec a sql and throw error message
 * 
 * @param curDB sqlite3 db
 * @param sql sql statement
 * @param cb callback for sqlite3_exec
 * @param ptr ptr for sqlite3_exec
 */
void db_exec(sqlite3 *curDB, const char *sql, sqlite_cb cb, void* ptr){
  if (curDB == nullptr){
    throw invalid_argument("ERROR in db_exec: db is nullptr");
  }

  char *err;
  if (0 != sqlite3_exec(curDB, sql, cb, ptr, &err)){
    string errMsg = err;
    sqlite3_free(err);
    sqlite3_close(db);
    db = nullptr;

    log_info("ERROR in db with sql: "s + string(sql));
    log_info("ERROR in db with err: "s + errMsg);

    throw runtime_error(errMsg);
  }
}

/**
 * init sqlite3 db and create table if not init
 */
void init_db(){
  if (db == nullptr){
    sqlite3_open(DB_NAME, &db);
    db_exec(db, createSQL, nullptr, nullptr);
  }
}

/**
 * close sqlite3 db
 */
void close_db(){
  if (db != nullptr){
    sqlite3_close(db);
    db = nullptr;
  }
}

/**
 * sql to get all infos
 */
const char *getAllSQL = 
"SELECT pixels, fingerprint, path "
"FROM image_storage"
;

/**
 * get all infos in db
 * 
 * @return all infos
 */
vector<ImageInfo> storage_infos(){
  init_db();

  vector<ImageInfo> infos;
  auto cb = [](void* infosPtr, int cols, char **data, char **column) -> int {
    ImageInfo info;
    info.pixels = stoi(data[0]);
    info.fingerprint = stoull(data[1]);
    info.path = string(data[2]);

    vector<ImageInfo> *infos = static_cast<vector<ImageInfo>*>(infosPtr);
    infos->push_back(info);

    return 0;
  };

  db_exec(db, getAllSQL, cb, &infos);

  return infos;
}

/**
 * sql to udpate info in db
 */
const char *updateSQL = 
"UPDATE image_storage "
"SET fingerprint = \"%llu\", pixels = %d, path = \"%s\" "
"WHERE path == \"%s\""
;

/**
 * update fingerprint and pixels by path
 * 
 * @param fp fingerprint
 * @param pixels
 * @param path path to image
 */
void update_info_by_path(
  fingerprint_t fp, 
  int pixels, 
  string newPath, 
  string oldPath
){
  init_db();

  char buffer[256];
  sprintf(buffer, updateSQL, fp, pixels, newPath.c_str(), oldPath.c_str());
  db_exec(db, buffer, nullptr, nullptr);
}

/**
 * sql to insert info
 */
const char *insertSQL =
"INSERT INTO image_storage "
"(fingerprint, pixels, path) "
"VALUES (\"%llu\", %d, \"%s\")"
;

/**
 * insert new information into database
 * 
 * @param fp fingerprint
 * @param pixels
 * @param path path to image
 */
void insert_info(fingerprint_t fp, int pixels, string path){
  init_db();

  char buffer[256];
  sprintf(buffer, insertSQL, fp, pixels, path.c_str());
  db_exec(db, buffer, nullptr, nullptr);
}

/**
 * sql to delete info in db
 */
const char *deleteSQL = 
"DELETE FROM image_storage "
"WHERE path == \"%s\""
;

/**
 * remove information from database by path
 *
 * @param path path to image
 */
void remove_info_by_path(string path){
  init_db();

  char buffer[256];
  sprintf(buffer, deleteSQL, path.c_str());
  db_exec(db, buffer, nullptr, nullptr);
}

/**
 * sql to clear db
 */
const char *clearSQL = "DROP TABLE image_storage";

/**
 * clear entire database
 */
void clear_db(){
  init_db();
  db_exec(db, clearSQL, nullptr, nullptr);
  close_db();
}

#ifdef GENERATE_UNIT_TEST

#include <catch2/catch.hpp>

SCENARIO("DB can insert, update, delete and load info"){
  GIVEN("an empty db"){
    clear_db();

    insert_info(0x0ull, 10, "storage/000/00.jpg");
    update_info_by_path(
      0xaa55aa55aa55aa55ull,
      11,
      "storage/000/00.jpg",
      "storage/000/00.jpg"
    );

    auto ret = storage_infos();
    REQUIRE(ret.size() == 1);
    REQUIRE(ret[0].fingerprint == 0xaa55aa55aa55aa55ull);
    REQUIRE(ret[0].pixels == 11);

    remove_info_by_path("storage/000/00.jpg");
  }
}

#endif

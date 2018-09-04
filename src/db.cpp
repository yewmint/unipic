#include "db.hpp"

#include <stdexcept>
#include <iostream>

using namespace std;

#define DB_NAME "unipic.db"

sqlite3 *db = nullptr;

const char *createSQL = 
"CREATE TABLE IF NOT EXISTS image_storage ("
  "id INT PRIMARY KEY AUTOINCREMENT,"
  "pixels INT NOT NULL,"
  "fingerprint INT NOT NULL,"
  "path TEXT NOT NULL"
");"
;

void init_db(){
  if (db == nullptr){
    sqlite3_open(DB_NAME, &db);
    sqlite3_exec(db, createSQL, NULL, NULL, NULL);
  }
}

void close_db(){
  if (db != nullptr){
    sqlite3_close(db);
    db = nullptr;
  }
}

const char *getAllSQL = "SELECT pixels, fingerprint, path FROM image_storage";

vector<ImageInfo> storage_infos(){
  init_db();

  vector<ImageInfo> infos;
  char *err;
  auto cb = [](void* infosPtr, int cols, char **data, char **column) -> int {
    ImageInfo info;
    info.pixels = stoi(data[0]);
    info.fingerprint = stoull(data[1]);
    info.path = string(data[2]);

    vector<ImageInfo> *infos = static_cast<vector<ImageInfo>*>(infosPtr);
    infos->push_back(info);

    return 0;
  };

  if (0 != sqlite3_exec(db, getAllSQL, cb, &infos, &err)){
    string errMsg = err;
    close_db();
    sqlite3_free(err);
    throw runtime_error(errMsg);
  }

  return infos;
}

#ifdef GENERATE_UNIT_TEST

#include "catch.hpp"

SCENARIO("DB can load storage infos", "[storage_infos]"){
  GIVEN("an empty db"){
    auto ret = storage_infos();
    REQUIRE(ret.size() == 0);
  }
}

#endif

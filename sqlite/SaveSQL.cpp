//
//  SaveSQL.cpp
//
//
//  Created by albatrus on 2014/06/10.
//
//

#include "SaveSQL.h"

#define dbName "Escapist.db"

USING_NS_CC;

//データベースを開く
int SaveSQL::sqliteOpen(sqlite3 **db){
    
    auto fileUtils = FileUtils::getInstance();
    std::string filePath = fileUtils->fullPathForFilename(dbName);
    
    return sqlite3_open(filePath.c_str(),db);
    
}


//keyから値を取り出す。
const char *SaveSQL::sqliteGetRankForScore(int score){
    
    __String *strValue = NULL;
    sqlite3 *db = NULL;
    if (sqliteOpen(&db) == SQLITE_OK) {
        
        //Escapist.dbテーブルからkeyに合わせたvalueを取り出す
        std::string sql_select = StringUtils::format("SELECT * FROM rank WHERE judge_min <= %d AND judge_max >= %d",score,score);
        sqlite3_stmt *stmt = NULL;
        if (sqlite3_prepare_v2(db, sql_select.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
            
            //sqlite3_bind_text(stmt, 1, key, -1, SQLITE_TRANSIENT);

            int ret = sqlite3_step(stmt);
            if (ret == SQLITE_DONE || ret == SQLITE_ROW) {
                
                const char *val = (const char*)sqlite3_column_text(stmt, 2);
                strValue = __String::create(val);
                
            }
            
            sqlite3_reset(stmt);
            
        }
        
        sqlite3_finalize(stmt);
        
    }
    
    sqlite3_close(db);
    
    
    //valueがある場合
    if (strValue != NULL) {
        
        return strValue->getCString();
        CCLOG("out");
    }
    
    return NULL;
}



//keyを元にValueの値を更新する
void SaveSQL::sqliteUpdateValueForKey(std::string _rank,const int No){
    
    
    std::string fullpath = CCFileUtils::getInstance()->getWritablePath();
    fullpath += dbName;
    sqlite3 *db = NULL;
    
    if (sqlite3_open(fullpath.c_str(), &db) == SQLITE_OK) {
        
        const char *sql_select = "UPDATE rank SET collect =? WHERE rank_name =? ";
        sqlite3_stmt *stmt = NULL;
        
        if (sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL) == SQLITE_OK) {
            
            sqlite3_bind_int(stmt, 1, No);
            sqlite3_bind_text(stmt, 2, _rank.c_str(), -1, SQLITE_TRANSIENT);
         
            if (sqlite3_step(stmt) == SQLITE_DONE) {
                
            }
            
            sqlite3_reset(stmt);
            
        }
        
        sqlite3_finalize(stmt);
        
    }
    
    sqlite3_close(db);
    
    
}



/*
void SaveSQL::sqliteUpdateValueForKey(std::string _rank){
    
    auto fileUtils = FileUtils::getInstance();
    std::string filePath = fileUtils->fullPathForFilename(dbName);
    sqlite3 *db = NULL;
    
    if (sqlite3_open(filePath.c_str(),&db) == SQLITE_OK) {
        
        std::string sql_select = StringUtils::format("UPDATE rank SET collect = %d WHERE rank_name = %s",1,_rank.c_str());
        
        CCLOG("%s",_rank.c_str());
       sqlite3_stmt *stmt = NULL;
        if (sqlite3_prepare_v2(db, sql_select.c_str(), -1, &stmt, NULL) == SQLITE_OK)
        {
            int ret = sqlite3_step(stmt);
            if (ret == SQLITE_DONE || ret == SQLITE_ROW) {
                
               sqlite3_column_int(stmt, 3);
            }
            
        }

        sqlite3_finalize(stmt);

    }

    sqlite3_close(db);

}
*/


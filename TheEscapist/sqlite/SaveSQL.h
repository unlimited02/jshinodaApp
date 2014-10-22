//
//  SaveSQL.h
//
//
//  Created by albatrus on 2014/06/10.
//
//

#ifndef __GitTutorial__SaveSQL__
#define __GitTutorial__SaveSQL__

#include "cocos2d.h"
#include "sqlite3.h"

class SaveSQL : public cocos2d::Node {
    
    
public:
    
    virtual  bool init();
    
    
    static int sqliteOpen(sqlite3 **db);
    
    static void sqliteCreateTable();
    static const char * sqliteGetRankForScore(int score);
   // static void sqliteSetValueForKey(const char *key,const char *value);
    
    static void sqliteUpdateValueForKey(std::string _rank,int No);
    
};


#endif /* defined(__GitTutorial__SaveSQL__) */
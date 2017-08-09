#include <zookeeper/zookeeper.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <zookeeper/zookeeper_log.h>
#include "election.h"
void watcher(zhandle_t* zh, int type, int state,
        const char* path, void* watcherCtx) {
    printf("Something happened.\n");
    printf("type: %d\n", type);
    printf("state: %d\n", state);
    printf("path: %s\n", path);
    printf("watcherCtx: %s\n", (char *)watcherCtx);
}


int main() {
 ElectionControl election;
 zhandle_t *zh;
 const char *server = "192.168.1.85:2181, 192.168.1.80:2181, 192.168.1.81:2182, 192.168.1.82:2181";
 if (election.Init(server, 30000) == false)
   return -1;
 while (election.Election() == false) {
   printf("Now election\n");
   sleep(10);
 }
 printf("Now working\n");
 while (true) {
   printf("working \n");
   sleep(10);
 }
 return 0;
}
  

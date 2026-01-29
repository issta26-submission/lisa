#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 543
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *users = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "users", users);
    cJSON *user1 = cJSON_CreateObject();
    cJSON_AddItemToArray(users, user1);
    cJSON *name1 = cJSON_CreateString("alice");
    cJSON_AddItemToObject(user1, "name", name1);
    cJSON *active1 = cJSON_CreateTrue();
    cJSON_AddItemToObject(user1, "active", active1);
    cJSON *user2 = cJSON_CreateObject();
    cJSON_AddItemToArray(users, user2);
    cJSON *name2 = cJSON_CreateString("bob");
    cJSON_AddItemToObject(user2, "name", name2);
    cJSON *active2 = cJSON_CreateFalse();
    cJSON_AddItemToObject(user2, "active", active2);

    // step 2: Configure
    cJSON *detached_users = cJSON_DetachItemFromObject(root, "users");
    cJSON *got_name1 = cJSON_GetObjectItem(user1, "name");
    char *name1_val = cJSON_GetStringValue(got_name1);
    cJSON *user3 = cJSON_CreateObject();
    cJSON_AddItemToArray(detached_users, user3);
    cJSON *name3 = cJSON_CreateString(name1_val);
    cJSON_AddItemToObject(user3, "name", name3);
    cJSON *active3 = cJSON_CreateTrue();
    cJSON_AddItemToObject(user3, "active", active3);
    cJSON_AddItemToObject(root, "users", detached_users);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
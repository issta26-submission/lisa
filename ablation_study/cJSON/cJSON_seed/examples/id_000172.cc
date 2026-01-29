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
//<ID> 172
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    cJSON *root = cJSON_CreateObject();
    const char *names_arr[] = { "alice", "bob", "carol" };
    cJSON *names = cJSON_CreateStringArray(names_arr, 3);
    cJSON_AddItemToObject(root, "names", names);
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", greeting);

    // step 2: Operate
    cJSON *new_greeting = cJSON_CreateString("hi");
    cJSON_ReplaceItemInObject(root, "greeting", new_greeting);

    // step 3: Validate
    cJSON *first = cJSON_GetArrayItem(names, 0);
    char *first_val = cJSON_GetStringValue(first);
    cJSON *greet_item = cJSON_GetObjectItem(root, "greeting");
    char *greet_val = cJSON_GetStringValue(greet_item);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
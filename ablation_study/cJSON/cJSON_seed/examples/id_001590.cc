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
//<ID> 1590
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *names = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "names", names);
    cJSON *name1 = cJSON_CreateString("Alice");
    cJSON_AddItemToArray(names, name1);
    cJSON *name2 = cJSON_CreateString("Bob");
    cJSON_AddItemToArray(names, name2);
    cJSON *name3 = cJSON_CreateString("Charlie");
    cJSON_AddItemToArray(names, name3);

    // step 2: Configure
    cJSON *active_flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "active", active_flag);
    cJSON *names_copy = cJSON_Duplicate(names, 1);
    cJSON_AddItemToObject(root, "names_copy", names_copy);

    // step 3: Operate & Validate
    int names_count = cJSON_GetArraySize(names);
    cJSON *count_item = cJSON_CreateNumber((double)names_count);
    cJSON_AddItemToObject(root, "names_count", count_item);
    cJSON *is_active_false = cJSON_CreateNumber((double)cJSON_IsFalse(active_flag));
    cJSON_AddItemToObject(root, "is_active_false", is_active_false);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
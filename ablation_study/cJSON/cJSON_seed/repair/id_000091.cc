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
//<ID> 91
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *obj0 = cJSON_CreateObject();
    cJSON *name0 = cJSON_CreateString("one");
    cJSON_AddItemToObject(obj0, "name", name0);
    cJSON_AddNumberToObject(obj0, "value", 1.0);
    cJSON_AddItemToArray(items, obj0);
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(items, n1);

    // step 2: Configure
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    cJSON *dup_items = cJSON_Duplicate(got_items, 1);
    cJSON *dup_first = cJSON_GetArrayItem(dup_items, 0);
    cJSON *dup_name = cJSON_GetObjectItem(dup_first, "name");
    char *set_ret = cJSON_SetValuestring(dup_name, "ONE");
    cJSON_AddNumberToObject(dup_first, "added", 42.0);

    // step 3: Operate and Validate
    int orig_size = cJSON_GetArraySize(items);
    int dup_size = cJSON_GetArraySize(dup_items);
    cJSON_AddNumberToObject(root, "orig_size", (double)orig_size);
    cJSON_AddNumberToObject(root, "dup_size", (double)dup_size);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(dup_items);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}
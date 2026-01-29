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
//<ID> 546
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
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, s1);
    cJSON *t1 = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, t1);
    cJSON *n1 = cJSON_CreateNumber(7.5);
    cJSON_AddItemToArray(items, n1);
    cJSON *name = cJSON_CreateString("original_name");
    cJSON_AddItemToObject(root, "name", name);

    // step 2: Configure
    cJSON *got_name = cJSON_GetObjectItem(root, "name");
    char *name_val = cJSON_GetStringValue(got_name);
    cJSON *name_clone = cJSON_CreateString(name_val);
    cJSON_AddItemToArray(items, name_clone);
    cJSON *detached_name = cJSON_DetachItemFromObject(root, "name");
    cJSON_AddItemToArray(items, detached_name);

    // step 3: Operate and Validate
    char *out1 = cJSON_PrintUnformatted(root);
    cJSON_free(out1);
    cJSON *root_dup = cJSON_Duplicate(root, 1);
    cJSON *dup_items = cJSON_GetObjectItem(root_dup, "items");
    cJSON *first_dup = cJSON_GetArrayItem(dup_items, 0);
    char *first_val = cJSON_GetStringValue(first_dup);
    cJSON *t2 = cJSON_CreateTrue();
    cJSON_AddItemToArray(dup_items, t2);
    char *out2 = cJSON_PrintUnformatted(root_dup);
    cJSON_free(out2);

    // step 4: Cleanup
    cJSON_Delete(root_dup);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}
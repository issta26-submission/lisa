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
//<ID> 549
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
    cJSON *obj1 = cJSON_CreateObject();
    cJSON *name1 = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(obj1, "name", name1);
    cJSON_AddItemToArray(items, obj1);
    cJSON *obj2 = cJSON_CreateObject();
    cJSON_AddItemToObject(obj2, "name", cJSON_CreateString("beta"));
    cJSON_AddItemToArray(items, obj2);
    cJSON *temp = cJSON_CreateObject();
    cJSON_AddItemToObject(temp, "name", cJSON_CreateString("gamma"));
    cJSON_AddItemToObject(root, "temp", temp);

    // step 2: Configure
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON *first_name = cJSON_GetObjectItemCaseSensitive(first_item, "name");
    char *first_name_val = cJSON_GetStringValue(first_name);
    cJSON *copied_name = cJSON_CreateString(first_name_val);
    cJSON_AddItemToObject(root, "copied_name", copied_name);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", true_item);
    cJSON *detached_temp = cJSON_DetachItemFromObject(root, "temp");
    cJSON_AddItemToArray(items, detached_temp);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON *root_dup = cJSON_Duplicate(root, 1);
    cJSON *dup_items = cJSON_GetObjectItem(root_dup, "items");
    cJSON *dup_first = cJSON_GetArrayItem(dup_items, 0);
    cJSON *dup_name = cJSON_GetObjectItem(dup_first, "name");
    char *dup_name_val = cJSON_GetStringValue(dup_name);
    cJSON_AddItemToObject(root_dup, "dup_copy", cJSON_CreateString(dup_name_val));
    char *out_dup = cJSON_PrintUnformatted(root_dup);
    cJSON_free(out_dup);
    cJSON_Delete(root_dup);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
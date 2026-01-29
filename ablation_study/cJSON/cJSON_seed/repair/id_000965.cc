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
//<ID> 965
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
    cJSON *entry0 = cJSON_CreateString("entry0");
    cJSON_AddItemToArray(items, entry0);

    // step 2: Configure
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "name", "child_one");
    cJSON_AddNumberToObject(child, "val", 7.0);
    cJSON *ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", ref);
    cJSON_AddItemToObject(root, "child", child);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(items, null_item);

    // step 3: Operate and Validate
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    char *first_str = cJSON_GetStringValue(first_item);
    cJSON *ref_obj = cJSON_GetObjectItem(root, "child_ref");
    cJSON *ref_name = cJSON_GetObjectItem(ref_obj, "name");
    char *ref_name_str = cJSON_GetStringValue(ref_name);
    char *snapshot = cJSON_PrintUnformatted(root);
    (void)first_str;
    (void)ref_name_str;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
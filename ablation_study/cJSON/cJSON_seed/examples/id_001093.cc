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
//<ID> 1093
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("device01");
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(settings, "name", name);
    cJSON_AddItemToObject(settings, "enabled", flag);
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON *arr = cJSON_CreateArray();
    cJSON *v0 = cJSON_CreateNumber(10.0);
    cJSON *v1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, v0);
    cJSON_AddItemToArray(arr, v1);
    cJSON_AddItemToObject(root, "values", arr);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(settings, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON *name_copy = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_bool replaced = cJSON_ReplaceItemInObject(settings, "enabled", true_item);
    (void)replaced;

    // step 3: Operate & Validate
    cJSON *second = v1;
    cJSON *newval = cJSON_CreateNumber(200.0);
    cJSON_bool replaced_via_ptr = cJSON_ReplaceItemViaPointer(arr, second, newval);
    (void)replaced_via_ptr;
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
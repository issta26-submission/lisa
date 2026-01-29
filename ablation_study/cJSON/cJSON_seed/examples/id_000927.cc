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
//<ID> 927
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON *num = cJSON_CreateNumber(10.0);
    cJSON *meta = cJSON_CreateObject();
    cJSON *name_orig = cJSON_CreateString("original");
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(meta, "name", name_orig);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *name_replacement = cJSON_CreateString("replaced");
    cJSON_bool replaced = cJSON_ReplaceItemInObject(meta, "name", name_replacement);

    // step 3: Operate & Validate
    cJSON *meta_ref = cJSON_GetObjectItem(root, "meta");
    cJSON *name_item = cJSON_GetObjectItem(meta_ref, "name");
    cJSON *items_ref = cJSON_GetObjectItem(root, "items");
    cJSON *num_item = cJSON_GetArrayItem(items_ref, 1);
    double newval = cJSON_SetNumberHelper(num_item, 42.5);
    cJSON *dup_str = cJSON_CreateString("alpha");

    // step 4: Cleanup
    (void)replaced;
    (void)newval;
    cJSON_Delete(dup_str);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}
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
//<ID> 695
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

    // step 2: Configure
    cJSON *obj1 = cJSON_CreateObject();
    cJSON_AddItemToArray(items, obj1);
    cJSON_AddNumberToObject(obj1, "id", 1.0);
    cJSON_AddStringToObject(obj1, "name", "alpha");
    cJSON *obj2 = cJSON_CreateObject();
    cJSON_AddItemToArray(items, obj2);
    cJSON_AddNumberToObject(obj2, "id", 2.0);
    cJSON_AddStringToObject(obj2, "name", "beta");
    cJSON *false_flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "status", false_flag);

    // step 3: Operate and Validate
    cJSON_bool obj1_is_obj = cJSON_IsObject(obj1);
    cJSON_AddBoolToObject(root, "obj1_is_object", obj1_is_obj);
    int item_count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "count", (double)item_count);
    cJSON *items_copy = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", items_copy);
    int copy_count = cJSON_GetArraySize(items_copy);
    cJSON_bool counts_equal = (item_count == copy_count);
    cJSON_AddBoolToObject(root, "counts_equal", counts_equal);

    // step 4: Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
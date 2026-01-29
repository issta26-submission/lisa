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
//<ID> 1413
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *str_item = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(array, num_item);
    cJSON_AddItemToArray(array, str_item);

    // step 2: Configure
    cJSON_AddItemToObject(root, "items", array);
    cJSON_AddNumberToObject(root, "total", 100.5);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate & Validate
    cJSON *items_ptr = cJSON_GetObjectItem(root, "items");
    cJSON *total_ptr = cJSON_GetObjectItem(root, "total");
    double total_val = cJSON_GetNumberValue(total_ptr);
    cJSON *dup_num = cJSON_Duplicate(num_item, 0);
    cJSON_AddItemReferenceToArray(items_ptr, dup_num);
    cJSON_AddNumberToObject(root, "total_plus", total_val + 10.0);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddRawToObject(root, "snapshot", snapshot);
    cJSON_free((void *)snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
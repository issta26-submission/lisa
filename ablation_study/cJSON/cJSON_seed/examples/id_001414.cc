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
//<ID> 1414
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *pi_item = cJSON_AddNumberToObject(root, "pi", 3.14159265);
    cJSON *active_item = cJSON_AddBoolToObject(root, "active", 1);
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddNumberToObject(nested, "val", 100.0);
    cJSON_AddItemToObject(root, "nested", nested);

    // step 2: Configure
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "array", array);
    cJSON *nested_ptr = cJSON_GetObjectItem(root, "nested");
    cJSON *dup_nested = cJSON_Duplicate(nested_ptr, 1);
    cJSON_AddItemToObject(root, "nested_copy", dup_nested);
    cJSON_AddItemReferenceToArray(array, nested_ptr);

    // step 3: Operate & Validate
    cJSON *pi_ptr = cJSON_GetObjectItem(root, "pi");
    double pi_val = cJSON_GetNumberValue(pi_ptr);
    cJSON_SetNumberHelper(pi_ptr, pi_val + 1.0);
    cJSON *nested_copy_ptr = cJSON_GetObjectItem(root, "nested_copy");
    cJSON *nested_copy_val = cJSON_GetObjectItem(nested_copy_ptr, "val");
    double new_val = cJSON_GetNumberValue(nested_copy_val) + cJSON_GetNumberValue(pi_ptr);
    cJSON_SetNumberHelper(nested_copy_val, new_val);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddRawToObject(root, "snapshot", snapshot);
    cJSON_free((void *)snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
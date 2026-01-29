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
//<ID> 1418
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
    cJSON_AddItemToObject(root, "array", arr);
    cJSON *num_item = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "base", num_item);
    cJSON_AddItemReferenceToArray(arr, num_item);
    cJSON_AddNumberToObject(root, "added_num", 2.5);
    cJSON_AddBoolToObject(root, "flag", 1);

    // step 2: Configure
    cJSON *dup_num = cJSON_Duplicate(num_item, 0);
    cJSON_AddItemToArray(arr, dup_num);

    // step 3: Operate & Validate
    cJSON *retrieved = cJSON_GetObjectItem(root, "added_num");
    double added_val = cJSON_GetNumberValue(retrieved);
    double base_val = cJSON_GetNumberValue(num_item);
    cJSON_SetNumberHelper(num_item, base_val + added_val);
    int arr_size = cJSON_GetArraySize(cJSON_GetObjectItem(root, "array"));
    cJSON_AddNumberToObject(root, "array_size", (double)arr_size);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
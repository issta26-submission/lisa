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
//<ID> 326
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
    cJSON *num0 = cJSON_CreateNumber(10.0);
    cJSON *num1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    cJSON *dup_first = cJSON_Duplicate(first_item, 0);
    cJSON_bool equal_first_dup = cJSON_Compare(first_item, dup_first, 1);
    cJSON *equal_node = cJSON_CreateBool(equal_first_dup);
    cJSON_AddItemToObject(meta, "first_equals_duplicate", equal_node);

    // step 3: Operate and Validate
    cJSON_DeleteItemFromArray(arr, 1);
    double first_value = cJSON_GetNumberValue(first_item);
    cJSON *reported_first = cJSON_CreateNumber(first_value);
    cJSON_AddItemToObject(meta, "reported_first", reported_first);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_first);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
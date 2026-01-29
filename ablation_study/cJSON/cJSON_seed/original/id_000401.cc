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
//<ID> 401
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *initial_value = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(root, "value1", initial_value);
    cJSON *arr = cJSON_AddArrayToObject(root, "arr");
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON *n2 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(20.0);
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "value1", replacement);
    const cJSON *value_lookup = cJSON_GetObjectItemCaseSensitive(root, "value1");
    cJSON_bool value_is_invalid = cJSON_IsInvalid(value_lookup);

    // step 3: Operate
    cJSON_DeleteItemFromArray(arr, 1);
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    double second_val = cJSON_GetNumberValue(second_item);

    // step 4: Validate and Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    double value_num = cJSON_GetNumberValue(value_lookup);
    cJSON_AddNumberToObject(root, "sum", value_num + second_val);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
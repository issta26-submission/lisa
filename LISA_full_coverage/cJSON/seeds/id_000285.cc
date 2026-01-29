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
//<ID> 285
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
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *num2 = cJSON_CreateNumber(-3.14);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure (create a reference to the existing number item)
    cJSON_AddItemReferenceToArray(arr, num1);

    // step 3: Operate and Validate
    cJSON *item0 = cJSON_GetArrayItem(arr, 0);
    cJSON *item2 = cJSON_GetArrayItem(arr, 2);
    double first_val = cJSON_GetNumberValue(item0);
    cJSON_bool flag_is_bool = cJSON_IsBool(flag);
    char *dump = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON_AddNumberToObject(root, "first_value", first_val);
    cJSON_AddBoolToObject(root, "flag_is_bool", flag_is_bool);

    // step 4: Cleanup
    cJSON_free(dump);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66;
}
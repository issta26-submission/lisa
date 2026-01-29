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
//<ID> 483
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
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *num0 = cJSON_CreateNumber(10.0);
    cJSON *num1 = cJSON_CreateNumber(20.5);
    cJSON *num2 = cJSON_CreateNumber(3.5);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON *name = cJSON_CreateString("sample_set");
    cJSON_AddItemToObject(root, "name", name);

    // step 2: Configure
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    double second_value = cJSON_GetNumberValue(second_item);
    cJSON *computed = cJSON_CreateNumber(second_value * 2.0 + 1.0);
    cJSON_AddItemToObject(root, "computed", computed);

    // step 3: Operate
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    cJSON *sum_item = cJSON_CreateNumber(first_value + second_value);
    cJSON_AddItemToObject(root, "sum_first_second", sum_item);
    cJSON *tag = cJSON_CreateString("processed");
    cJSON_AddItemToArray(arr, tag);

    // step 4: Validate and Cleanup
    cJSON *validated = cJSON_GetObjectItem(root, "computed");
    double validated_value = cJSON_GetNumberValue(validated);
    (void)validated_value;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
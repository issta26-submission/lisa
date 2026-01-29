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
//<ID> 18
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = cJSON_Parse("{\"nested\": 123, \"flag\": true}");
    cJSON *bfalse = cJSON_CreateFalse();

    // step 2: Configure
    cJSON *arr = cJSON_AddArrayToObject(root, "arr");
    cJSON_AddItemToArray(arr, parsed);
    cJSON_AddItemToArray(arr, bfalse);
    cJSON *str = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str);

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *nested_item = cJSON_GetObjectItem(first, "nested");
    double nested_value = cJSON_GetNumberValue(nested_item);
    cJSON_bool is_false = cJSON_IsFalse(cJSON_GetArrayItem(arr, 1));
    double sum_value = nested_value + (double)size + (double)(!is_false);
    cJSON *sum_num = cJSON_CreateNumber(sum_value);
    cJSON_AddItemToObject(root, "sum", sum_num);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}
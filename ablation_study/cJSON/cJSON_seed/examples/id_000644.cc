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
//<ID> 644
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *arr = cJSON_CreateArray();
    cJSON *item_a = cJSON_CreateString("alpha");
    cJSON *item_b = cJSON_CreateNull();
    cJSON *num_item = cJSON_CreateNumber(42.0);

    // step 2: Configure
    cJSON_AddItemToArray(arr, item_a);
    cJSON_AddItemToArray(arr, item_b);
    cJSON_AddItemToArray(arr, num_item);
    cJSON *replacement = cJSON_CreateString("replaced_alpha");
    cJSON_ReplaceItemViaPointer(arr, item_a, replacement);

    // step 3: Operate & Validate
    const int buf_len = 256;
    char buffer[buf_len];
    memset(buffer, 0, buf_len);
    cJSON_PrintPreallocated(arr, buffer, buf_len, 1);
    char *snapshot = cJSON_PrintUnformatted(arr);
    int array_count = cJSON_GetArraySize(arr);
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    cJSON_bool second_is_null = cJSON_IsNull(second_item);
    cJSON *third_item = cJSON_GetArrayItem(arr, 2);
    double third_value = cJSON_GetNumberValue(third_item);
    (void)version;
    (void)second_is_null;
    (void)third_value;
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(arr);
    // API sequence test completed successfully
    return 66;
}
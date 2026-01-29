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
//<ID> 898
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *array = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(2.71);
    cJSON *raw_item = cJSON_CreateRaw("\"raw-text\"");
    // step 2: Configure
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddItemToArray(array, raw_item);
    // step 3: Operate & Validate
    int size_before = cJSON_GetArraySize(array);
    cJSON *size_marker1 = cJSON_CreateNumber((double)size_before);
    cJSON_AddItemToArray(array, size_marker1);
    char *raw_value = cJSON_GetStringValue(raw_item);
    cJSON *raw_clone = cJSON_CreateRaw(raw_value);
    cJSON_AddItemToArray(array, raw_clone);
    int size_after = cJSON_GetArraySize(array);
    cJSON *size_marker2 = cJSON_CreateNumber((double)size_after);
    cJSON_AddItemToArray(array, size_marker2);
    // step 4: Cleanup
    cJSON_Delete(array);
    // API sequence test completed successfully
    return 66;
}
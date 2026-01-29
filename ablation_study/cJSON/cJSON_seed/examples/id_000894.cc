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
//<ID> 894
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *arr = cJSON_CreateArray();
    cJSON *raw1 = cJSON_CreateRaw("\"sensor-raw\"");
    cJSON *num1 = cJSON_CreateNumber(12.34);
    cJSON *raw2 = cJSON_CreateRaw("\"status-raw\"");

    // step 2: Configure
    cJSON_AddItemToArray(arr, raw1);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, raw2);

    // step 3: Operate & Validate
    int size_before = cJSON_GetArraySize(arr);
    char *val_raw1 = cJSON_GetStringValue(raw1);
    cJSON *size_marker = cJSON_CreateNumber((double)size_before);
    cJSON_AddItemToArray(arr, size_marker);
    cJSON *raw_copy = cJSON_CreateRaw(val_raw1);
    cJSON_AddItemToArray(arr, raw_copy);
    int size_after = cJSON_GetArraySize(arr);
    char *val_raw2 = cJSON_GetStringValue(raw2);
    cJSON *final_count = cJSON_CreateNumber((double)size_after);
    cJSON_AddItemToArray(arr, final_count);
    char *snapshot = cJSON_PrintUnformatted(arr);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(arr);
    // API sequence test completed successfully
    return 66;
}
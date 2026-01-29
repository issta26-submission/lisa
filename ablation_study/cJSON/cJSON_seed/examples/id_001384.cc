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
//<ID> 1384
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
    cJSON *false_item = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToObject(root, "flags", arr);
    cJSON *bool_in_obj = cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON *num_item = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, num_item);

    // step 3: Operate & Validate
    cJSON_bool is_array = cJSON_IsArray(arr);
    int size = cJSON_GetArraySize(arr);
    double updated_value = (double)(size + (int)is_array);
    cJSON_SetNumberHelper(num_item, updated_value);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddRawToObject(root, "snapshot", snapshot);
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    cJSON_AddRawToObject(root, "snapshot_pre", buffer);
    cJSON_free((void *)snapshot);
    cJSON_free((void *)buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1385
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON *s1 = cJSON_CreateString("hello");

    // step 2: Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, cJSON_CreateFalse());
    cJSON_AddItemToObject(meta, "data", arr);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "greeting", s1);
    cJSON_AddBoolToObject(root, "enabled", 1);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddRawToObject(root, "snapshot", snapshot);
    cJSON_free((void *)snapshot);

    // step 3: Operate & Validate
    cJSON_bool is_arr = cJSON_IsArray(arr);
    double current = cJSON_GetNumberValue(n1);
    cJSON_SetNumberHelper(n1, current + (double)is_arr);
    const char *greet = cJSON_GetStringValue(s1);
    cJSON *dup = cJSON_CreateString(greet);
    cJSON_AddItemToArray(arr, dup);
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    cJSON_AddRawToObject(root, "snapshot_prealloc", buffer);
    cJSON_free((void *)buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
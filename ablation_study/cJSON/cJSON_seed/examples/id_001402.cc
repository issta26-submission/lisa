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
//<ID> 1402
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
    cJSON *num0 = cJSON_CreateNumber(1.0);
    cJSON *num1 = cJSON_CreateNumber(2.0);
    cJSON *str = cJSON_CreateString("item");
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, str);
    cJSON_AddItemToObjectCS(root, "array", arr);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_bool replaced_flag = cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *flag_ptr = cJSON_AddTrueToObject(root, "flag");
    int size = cJSON_GetArraySize(arr);
    cJSON *size_plus = cJSON_CreateNumber((double)size + 3.0);
    cJSON_AddItemToObjectCS(root, "size_plus", size_plus);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    int buf_len = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    double orig = cJSON_GetNumberValue(num0);
    double updated = orig + (double)replaced_flag + (double)cJSON_GetArraySize(arr);
    cJSON_SetNumberHelper(num0, updated);
    cJSON_AddRawToObject(root, "snapshot_raw", snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free((void *)snapshot);
    cJSON_free((void *)buffer);
    // API sequence test completed successfully
    return 66;
}
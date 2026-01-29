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
//<ID> 360
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double nums[3] = {1.1, 2.2, 3.3};
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 2: Configure
    int count = cJSON_GetArraySize(arr);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *dbl2 = cJSON_CreateNumber(second_val * 2.0);
    cJSON_AddItemToArray(arr, dbl2);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    int new_count = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count", (double)new_count);
    int buf_len = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, buf_len);
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 0);
    cJSON_AddNumberToObject(root, "prealloc_ok", (double)pre_ok);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
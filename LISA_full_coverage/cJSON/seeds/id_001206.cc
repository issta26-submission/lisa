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
//<ID> 1206
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *arr = cJSON_CreateArray();
    cJSON *s0 = cJSON_CreateStringReference("zero");
    cJSON *s1 = cJSON_CreateStringReference("one");
    cJSON *s2 = cJSON_CreateStringReference("two");
    cJSON_InsertItemInArray(arr, 0, s0);
    cJSON_InsertItemInArray(arr, 1, s2);
    cJSON_InsertItemInArray(arr, 1, s1);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON *s3 = cJSON_CreateStringReference("three");
    int end_index = cJSON_GetArraySize(arr);
    cJSON_InsertItemInArray(arr, end_index, s3);

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, buf, buf_len, 1);
    int arr_size = cJSON_GetArraySize(arr);

    // step 4: Validate & Cleanup
    (void)arr_size;
    (void)pre_ok;
    cJSON_free(pretty);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1204
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *s1 = cJSON_CreateStringReference("one");
    cJSON_AddItemToArray(arr, s1);
    cJSON *s2 = cJSON_CreateStringReference("zero");
    cJSON_InsertItemInArray(arr, 0, s2);
    cJSON *n = cJSON_CreateNumber(100.0);
    cJSON_AddItemToArray(arr, n);

    // step 2: Configure
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "items_copy", dup_arr);
    cJSON *extra = cJSON_CreateStringReference("extra");
    cJSON_AddItemToArray(dup_arr, extra);

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_bool arr_flag = cJSON_IsArray(arr);
    const char *first_val = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 0));
    (void)arr_flag;
    (void)first_val;

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
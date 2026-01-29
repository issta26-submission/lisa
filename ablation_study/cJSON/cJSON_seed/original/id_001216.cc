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
//<ID> 1216
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

    // step 2: Configure
    cJSON *s = cJSON_CreateString("initial");
    char *new_val = cJSON_SetValuestring(s, "modified");
    cJSON_AddItemToArray(arr, s);
    cJSON *t = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, t);
    cJSON *n = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, n);
    cJSON *explicit_null = cJSON_AddNullToObject(root, "explicit_null");

    // step 3: Operate
    int arr_size = cJSON_GetArraySize(arr);
    char *printed = cJSON_PrintUnformatted(root);
    int buf_len = 512;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON *dup = cJSON_Duplicate(root, 1);
    char *dup_printed = cJSON_PrintUnformatted(dup);

    // step 4: Validate & Cleanup
    (void)new_val;
    (void)arr_size;
    (void)prealloc_ok;
    cJSON_free(printed);
    cJSON_free(dup_printed);
    cJSON_free(buf);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
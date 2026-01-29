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
//<ID> 1139
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
    cJSON *sref = cJSON_CreateStringReference("original");
    cJSON_AddItemToArray(arr, sref);
    cJSON *t = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, t);
    cJSON *n = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, n);

    // step 2: Configure
    cJSON_AddNullToObject(root, "optional");
    cJSON *replacement = cJSON_CreateStringReference("replaced");
    cJSON_bool did_replace = cJSON_ReplaceItemInArray(arr, 0, replacement);

    // step 3: Operate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    const char *first_val = cJSON_GetStringValue(first);
    cJSON_AddStringToObject(root, "first_value", first_val);
    int count = cJSON_GetArraySize(arr);
    double third_num = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 2));
    (void)did_replace;
    (void)count;
    (void)third_num;

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
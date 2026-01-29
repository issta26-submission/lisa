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
//<ID> 1231
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
    cJSON *num = cJSON_CreateNumber(42.5);
    cJSON *sref = cJSON_CreateStringReference("sample");
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, sref);
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "dup_items", dup_arr);
    cJSON *extra = cJSON_CreateNumber(7.5);
    cJSON_AddItemToArray(dup_arr, extra);

    // step 3: Operate and Validate
    cJSON *it0 = cJSON_GetArrayItem(arr, 0);
    cJSON *it1 = cJSON_GetArrayItem(arr, 1);
    double v0 = cJSON_GetNumberValue(it0);
    const char *s = cJSON_GetStringValue(it1);
    double sval_first = (s && s[0]) ? (double)(unsigned char)s[0] : 0.0;
    cJSON *flag_ptr = cJSON_GetObjectItem(root, "flag");
    cJSON_bool flag_is_bool = cJSON_IsBool(flag_ptr);
    cJSON_bool flag_is_false = cJSON_IsFalse(flag_ptr);
    double total = v0 + sval_first + (double)flag_is_bool + (double)flag_is_false;
    cJSON *result = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "total", result);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
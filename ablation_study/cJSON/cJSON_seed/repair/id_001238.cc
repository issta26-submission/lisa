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
//<ID> 1238
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
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *n1 = cJSON_CreateNumber(4.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON *sref = cJSON_CreateStringReference("refstr");
    cJSON_AddItemToArray(arr, sref);

    // step 2: Configure
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_bool flag_is_bool = cJSON_IsBool(flag);
    cJSON_bool flag_is_false = cJSON_IsFalse(flag);
    cJSON *n2 = cJSON_CreateNumber(cJSON_GetNumberValue(n1) + (double)flag_is_bool + (double)flag_is_false);
    cJSON_AddItemToObject(root, "sum", n2);

    // step 3: Operate and Validate
    cJSON *item0 = cJSON_GetArrayItem(arr, 0);
    cJSON *item1 = cJSON_GetArrayItem(arr, 1);
    double v0 = cJSON_GetNumberValue(item0);
    const char *sval = cJSON_GetStringValue(item1);
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "dup", dup_arr);
    double final_total = v0 + cJSON_GetNumberValue(n2);
    cJSON_AddNumberToObject(root, "final", final_total);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
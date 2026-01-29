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
//<ID> 1230
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
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *n = cJSON_CreateNumber(12.5);
    cJSON *sref = cJSON_CreateStringReference("example-ref");
    cJSON *f = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, n);
    cJSON_AddItemToArray(arr, sref);
    cJSON_AddItemToArray(arr, f);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta, "flag", flag);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *third = cJSON_GetArrayItem(arr, 2);
    double val = cJSON_GetNumberValue(first);
    cJSON_bool isbool = cJSON_IsBool(third);
    cJSON_bool isfalse = cJSON_IsFalse(third);
    double result = val + (double)isbool - (double)isfalse;
    cJSON *resnum = cJSON_CreateNumber(result);
    cJSON_AddItemToObject(root, "result", resnum);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1226
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
    cJSON *s = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, s);
    cJSON *n = cJSON_CreateNumber(7.5);
    cJSON_AddItemToArray(arr, n);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *base = cJSON_CreateNumber(3.25);
    cJSON_AddItemToObject(meta, "base", base);

    // step 2: Configure
    cJSON *label = cJSON_CreateString("configured");
    cJSON_AddItemToObject(meta, "label", label);
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "dup_items", dup_arr);

    // step 3: Operate and Validate
    cJSON *it0 = cJSON_GetArrayItem(arr, 0);
    cJSON *it1 = cJSON_GetArrayItem(arr, 1);
    cJSON_bool it0_is_string = cJSON_IsString(it0);
    double v1 = cJSON_GetNumberValue(it1);
    cJSON *dup_it1 = cJSON_GetArrayItem(dup_arr, 1);
    double vdup1 = cJSON_GetNumberValue(dup_it1);
    cJSON *meta_base = cJSON_GetObjectItem(meta, "base");
    double vbase = cJSON_GetNumberValue(meta_base);
    double total = v1 + vdup1 + vbase + (double)it0_is_string;
    cJSON *final = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "final", final);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
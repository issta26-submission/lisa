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
//<ID> 1234
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
    cJSON_AddItemToObject(root, "data", arr);
    cJSON *n1 = cJSON_CreateNumber(42.5);
    cJSON *n2 = cJSON_CreateNumber(-3.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON *sref = cJSON_CreateStringReference("ref_string");
    cJSON_AddItemToArray(arr, sref);
    cJSON *bf = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, bf);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *base = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(meta, "base", base);
    cJSON *label = cJSON_CreateStringReference("configured");
    cJSON_AddItemToObject(meta, "label", label);

    // step 3: Operate and Validate
    cJSON *it0 = cJSON_GetArrayItem(arr, 0);
    cJSON *it1 = cJSON_GetArrayItem(arr, 1);
    cJSON *it2 = cJSON_GetArrayItem(arr, 2);
    cJSON *meta_base = cJSON_GetObjectItem(meta, "base");
    double v0 = cJSON_GetNumberValue(it0);
    double v1 = cJSON_GetNumberValue(it1);
    double vb = cJSON_GetNumberValue(meta_base);
    cJSON_bool is_bf_bool = cJSON_IsBool(bf);
    cJSON_bool is_bf_false = cJSON_IsFalse(bf);
    cJSON_bool is_it2_bool = cJSON_IsBool(it2);
    double total = v0 + v1 + vb + (double)is_bf_bool + (double)is_bf_false + (double)is_it2_bool;
    cJSON *ctotal = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "total", ctotal);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
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
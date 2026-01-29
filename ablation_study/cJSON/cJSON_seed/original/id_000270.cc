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
//<ID> 270
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float fvals[3] = { 1.0f, 2.5f, -0.75f };
    cJSON *farr = cJSON_CreateFloatArray(fvals, 3);
    const char *strs_arr[] = { "alpha", "beta" };
    cJSON *sarr = cJSON_CreateStringArray(strs_arr, 2);
    cJSON_AddItemToObject(root, "floats", farr);
    cJSON_AddItemToObject(root, "strings", sarr);
    cJSON *note = cJSON_AddStringToObject(root, "note", "created-by-test");

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "inner", "value");
    cJSON_AddNumberToObject(meta, "count", 3.0);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *sarr_dup = cJSON_Duplicate(sarr, 1);
    cJSON_bool cmp_sarr_dup = cJSON_Compare(sarr, sarr_dup, 1);

    // step 3: Operate and Validate
    cJSON_bool farr_is_array = cJSON_IsArray(farr);
    cJSON_bool sarr_is_array = cJSON_IsArray(sarr);
    int fcount = cJSON_GetArraySize(farr);
    int scount = cJSON_GetArraySize(sarr);
    cJSON_AddNumberToObject(root, "fcount", (double)fcount);
    cJSON_AddNumberToObject(root, "scount", (double)scount);
    cJSON_AddBoolToObject(root, "farr_is_array", farr_is_array);
    cJSON_AddBoolToObject(root, "sarr_is_array", sarr_is_array);
    cJSON_AddBoolToObject(root, "sarr_equal_dup", cmp_sarr_dup);

    // step 4: Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);
    cJSON_Delete(sarr_dup);

    // API sequence test completed successfully
    return 66;
}
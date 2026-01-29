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
//<ID> 518
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToArray(values, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(30.0));
    cJSON_AddItemToObject(root, "values", values);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", false_item);
    cJSON *verified_item = cJSON_AddTrueToObject(root, "verified");

    // step 2: Configure
    int sz_before = cJSON_GetArraySize(values);
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 0));
    cJSON *meta = cJSON_CreateObject();
    double computed = first_val + (double)sz_before;
    cJSON_AddNumberToObject(meta, "computed", computed);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate
    cJSON_DeleteItemFromArray(values, 1);
    int sz_after = cJSON_GetArraySize(values);
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t buf_len = 64 + (size_t)sz_after;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);

    // step 4: Validate & Cleanup
    double extracted = cJSON_GetNumberValue(cJSON_GetObjectItem(meta, "computed"));
    (void)verified_item;
    (void)false_item;
    (void)extracted;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
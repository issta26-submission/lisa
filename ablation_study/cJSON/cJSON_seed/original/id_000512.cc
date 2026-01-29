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
//<ID> 512
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *n1 = cJSON_CreateNumber(10.5);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *s3 = cJSON_CreateString("thirty");
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    cJSON_AddItemToArray(items, s3);
    cJSON_AddTrueToObject(root, "enabled");
    cJSON *f4 = cJSON_CreateFalse();
    cJSON_AddItemToArray(items, f4);

    // step 2: Configure
    int size_before = cJSON_GetArraySize(items);
    cJSON *second = cJSON_GetArrayItem(items, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "sum_estimate", second_val + 1.0);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate
    cJSON_DeleteItemFromArray(items, 1);
    int size_after = cJSON_GetArraySize(items);
    size_t buf_len = 64 + (size_t)size_after;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    (void)size_before;
    (void)second_val;
    (void)size_after;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
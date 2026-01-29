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
//<ID> 515
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
    cJSON *num1 = cJSON_CreateNumber(10.5);
    cJSON *num2 = cJSON_CreateNumber(20.0);
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, num1);
    cJSON_AddItemToArray(items, num2);
    cJSON_AddItemToArray(items, str1);
    cJSON_AddItemToObject(root, "items", items);
    cJSON *meta = cJSON_CreateObject();
    cJSON *disabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(meta, "enabled", disabled);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddTrueToObject(root, "ok");

    // step 2: Configure
    int count_before = cJSON_GetArraySize(items);
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    double second_value = cJSON_GetNumberValue(second_item);

    // step 3: Operate
    cJSON_DeleteItemFromArray(items, 0);
    int count_after = cJSON_GetArraySize(items);
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t buf_len = 48 + (size_t)count_after;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);

    // step 4: Validate & Cleanup
    (void)count_before;
    (void)count_after;
    (void)second_value;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
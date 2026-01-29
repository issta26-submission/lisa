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
//<ID> 511
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
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(30.0));
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(items, false_item);
    int size_before = cJSON_GetArraySize(items);
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(items, 0));

    // step 2: Configure
    cJSON_DeleteItemFromArray(items, 1);
    cJSON_AddTrueToObject(root, "status");
    cJSON *note = cJSON_CreateString("snapshot");
    cJSON_AddItemToObject(root, "note", note);
    int size_after = cJSON_GetArraySize(items);
    cJSON_bool false_still_present = cJSON_IsFalse(cJSON_GetArrayItem(items, 2));

    // step 3: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t buf_len = 64 + (size_t)size_after * 16;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    double second_element_value = cJSON_GetNumberValue(cJSON_GetArrayItem(items, 1));
    cJSON *status_item = cJSON_GetObjectItem(root, "status");
    cJSON_bool status_is_true = cJSON_IsTrue(status_item);

    // step 4: Validate & Cleanup
    (void)size_before;
    (void)first_val;
    (void)size_after;
    (void)false_still_present;
    (void)second_element_value;
    (void)status_is_true;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
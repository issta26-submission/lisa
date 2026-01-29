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
//<ID> 608
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    const char *items_arr[] = { "alpha", "beta", "gamma" };
    cJSON *items = cJSON_CreateStringArray(items_arr, 3);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToObject(root, "is_active", true_item);
    cJSON_AddItemToObject(root, "is_closed", false_item);

    // step 2: Configure
    int original_count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "count", (double)original_count);
    cJSON_AddStringToObject(root, "owner", "tester");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool has_items = cJSON_HasObjectItem(parsed, "items");
    cJSON *parsed_true = cJSON_GetObjectItem(parsed, "is_active");
    cJSON *parsed_false = cJSON_GetObjectItem(parsed, "is_closed");
    cJSON_bool is_true = cJSON_IsTrue(parsed_true);
    cJSON_bool is_false = cJSON_IsFalse(parsed_false);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;count=%d;has_items=%d;is_true=%d;is_false=%d",
            version ? version : "null",
            original_count,
            (int)has_items,
            (int)is_true,
            (int)is_false);
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
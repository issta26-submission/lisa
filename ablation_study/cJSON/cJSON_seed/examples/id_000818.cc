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
//<ID> 818
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *items_arr[] = { "alpha", "beta", "gamma" };
    cJSON *array = cJSON_CreateStringArray(items_arr, 3);
    cJSON_AddItemToObject(root, "items", array);
    cJSON *flag = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "confirmed", true_item);
    cJSON_AddNullToObject(root, "missing");

    // step 2: Configure
    cJSON *picked = cJSON_GetArrayItem(array, 1);
    const char *picked_str = cJSON_GetStringValue(picked);
    cJSON *selected_item = cJSON_CreateString(picked_str);
    cJSON_AddItemToObject(root, "selected", selected_item);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_items = cJSON_GetObjectItem(parsed, "items");
    cJSON *parsed_second = cJSON_GetArrayItem(parsed_items, 1);
    const char *extracted = cJSON_GetStringValue(parsed_second);
    cJSON *extracted_dup = cJSON_CreateString(extracted);
    cJSON_AddItemToObject(parsed, "extracted_dup", extracted_dup);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
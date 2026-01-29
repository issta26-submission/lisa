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
//<ID> 301
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"a\":1,\"b\":2}";
    size_t json_len = sizeof(json) - 1;
    cJSON *parsed = cJSON_ParseWithLength(json, json_len);
    cJSON *root = cJSON_CreateObject();
    const int nums[] = {10, 20, 30};
    cJSON *int_arr = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "numbers", int_arr);

    // step 2: Configure
    cJSON_AddNullToObject(root, "maybe_null");
    cJSON_AddFalseToObject(root, "enabled");
    cJSON *item_a = cJSON_GetObjectItem(parsed, "a");
    double a_value = cJSON_GetNumberValue(item_a);
    cJSON_AddNumberToObject(root, "parsed_a", a_value);
    cJSON_bool has_b = cJSON_HasObjectItem(parsed, "b");
    cJSON_AddBoolToObject(root, "parsed_has_b", has_b);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}
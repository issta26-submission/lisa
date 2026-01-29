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
//<ID> 307
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"a\":1,\"b\":[2,3],\"flag\":true}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);

    // step 2: Configure
    int numbers[] = {7, 8, 9};
    cJSON *int_array = cJSON_CreateIntArray(numbers, 3);
    cJSON_bool added_ints = cJSON_AddItemToObject(root, "ints", int_array);
    (void)added_ints;
    cJSON *maybe_null = cJSON_AddNullToObject(root, "maybe_null");
    cJSON *is_enabled_false = cJSON_AddFalseToObject(root, "is_enabled");

    // step 3: Operate and Validate
    cJSON_bool has_a = cJSON_HasObjectItem(root, "a");
    cJSON_bool has_missing = cJSON_HasObjectItem(root, "missing");
    cJSON *a_item = cJSON_GetObjectItem(root, "a");
    double a_value = cJSON_GetNumberValue(a_item);
    cJSON *a_copy = cJSON_AddNumberToObject(root, "a_copy", a_value);
    cJSON *a_exists_flag = cJSON_AddBoolToObject(root, "a_exists", has_a);
    cJSON *missing_exists_flag = cJSON_AddBoolToObject(root, "missing_exists", has_missing);
    char *printed = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
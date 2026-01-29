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
//<ID> 305
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"existing\":1}";
    cJSON *parsed = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    int numbers[] = {10, 20, 30};
    cJSON *int_array = cJSON_CreateIntArray(numbers, 3);
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *null_item = cJSON_AddNullToObject(root, "maybe_null");
    cJSON *false_item = cJSON_AddFalseToObject(root, "flag_false");

    // step 3: Operate and Validate
    cJSON_bool has_existing = cJSON_HasObjectItem(root, "existing");
    cJSON_AddBoolToObject(root, "has_existing", has_existing);
    cJSON_bool null_is_null = cJSON_IsNull(null_item);
    cJSON_AddBoolToObject(root, "null_is_null", null_is_null);
    cJSON_bool false_is_false = cJSON_IsFalse(false_item);
    cJSON_AddBoolToObject(root, "false_is_false", false_is_false);
    int array_size = cJSON_GetArraySize(int_array);
    cJSON_AddNumberToObject(root, "ints_count", (double)array_size);
    cJSON *existing = cJSON_GetObjectItem(parsed, "existing");
    double existing_value = cJSON_GetNumberValue(existing);
    cJSON_AddNumberToObject(root, "existing_value", existing_value);
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
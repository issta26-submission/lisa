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
//<ID> 303
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char json[] = "{\"present\":false,\"nullable\":null,\"value\":123}";
    size_t json_len = sizeof(json) - 1;
    cJSON *parsed = cJSON_ParseWithLength(json, json_len);
    int numbers[] = {10, 20, 30, 40};
    cJSON *int_arr = cJSON_CreateIntArray(numbers, 4);
    cJSON_AddItemToObject(root, "ints", int_arr);

    // step 2: Configure
    cJSON *null_added = cJSON_AddNullToObject(root, "added_null");
    cJSON *false_added = cJSON_AddFalseToObject(root, "added_false");
    cJSON *parsed_copy = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(root, "parsed_copy", parsed_copy);

    // step 3: Operate and Validate
    cJSON_bool root_has_ints = cJSON_HasObjectItem(root, "ints");
    cJSON_bool parsed_has_present = cJSON_HasObjectItem(parsed, "present");
    cJSON_AddBoolToObject(root, "root_has_ints", root_has_ints);
    cJSON_AddBoolToObject(root, "parsed_has_present", parsed_has_present);
    char *printed = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(256);
    memset(buffer, 0, 256);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
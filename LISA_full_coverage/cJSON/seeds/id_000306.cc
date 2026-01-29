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
//<ID> 306
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"values\":[1,2,3],\"flag\":false}";
    size_t json_len = sizeof(json) - 1;
    cJSON *parsed = cJSON_ParseWithLength(json, json_len);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    int numbers[] = {10, 20, 30, 40};
    cJSON *int_array = cJSON_CreateIntArray(numbers, 4);
    cJSON_AddItemToObject(root, "input", parsed);
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON *n = cJSON_AddNullToObject(root, "empty");
    cJSON *f = cJSON_AddFalseToObject(root, "disabled");

    // step 3: Operate and Validate
    cJSON_bool has_ints = cJSON_HasObjectItem(root, "ints");
    cJSON_bool has_empty = cJSON_HasObjectItem(root, "empty");
    cJSON *first_int = cJSON_GetArrayItem(int_array, 0);
    double first_val = cJSON_GetNumberValue(first_int);
    cJSON_AddNumberToObject(root, "first_val", first_val);
    cJSON_AddBoolToObject(root, "has_ints", has_ints);
    cJSON_AddBoolToObject(root, "has_empty", has_empty);
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
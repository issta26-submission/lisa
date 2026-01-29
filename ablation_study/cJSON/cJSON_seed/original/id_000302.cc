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
//<ID> 302
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const int numbers[] = {10, 20, 30};
    cJSON *int_array = cJSON_CreateIntArray(numbers, 3);
    cJSON_AddItemToObject(root, "numbers", int_array);
    const char json[] = "{\"parsed\":false}";
    cJSON *parsed = cJSON_ParseWithLength(json, (size_t)(sizeof(json) - 1));
    cJSON *parsed_dup = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(root, "parsed_dup", parsed_dup);

    // step 2: Configure
    cJSON *null_entry = cJSON_AddNullToObject(root, "empty");
    cJSON *false_entry = cJSON_AddFalseToObject(root, "flag_false");
    cJSON_bool parsed_has_key = cJSON_HasObjectItem(parsed, "parsed");

    // step 3: Operate and Validate
    int arr_size = cJSON_GetArraySize(int_array);
    cJSON *first_item = cJSON_GetArrayItem(int_array, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    cJSON_AddNumberToObject(root, "first_number", first_value);
    cJSON_AddBoolToObject(root, "parsed_exists", parsed_has_key);
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
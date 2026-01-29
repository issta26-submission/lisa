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
//<ID> 308
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int numbers_arr[4] = {10, 20, 30, 40};
    cJSON *ints = cJSON_CreateIntArray(numbers_arr, 4);
    cJSON_AddItemToObject(root, "numbers", ints);
    cJSON_AddNullToObject(root, "nothing");
    cJSON_AddFalseToObject(root, "is_disabled");

    // step 2: Configure
    const char *json_text = "{\"extracted\":256,\"flag\":true}";
    size_t json_len = strlen(json_text);
    cJSON *parsed = cJSON_ParseWithLength(json_text, json_len);
    cJSON *extracted_item = cJSON_GetObjectItem(parsed, "extracted");
    double extracted_value = cJSON_GetNumberValue(extracted_item);
    cJSON_AddNumberToObject(root, "extracted_value", extracted_value);
    cJSON_bool parsed_has_flag = cJSON_HasObjectItem(parsed, "flag");
    cJSON_AddBoolToObject(root, "parsed_has_flag", parsed_has_flag);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON_bool root_has_numbers = cJSON_HasObjectItem(root, "numbers");
    cJSON_AddBoolToObject(root, "root_has_numbers", root_has_numbers);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
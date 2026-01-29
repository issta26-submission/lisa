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
//<ID> 783
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"arr\": [1, 2, 3], \"value\": 42}";
    size_t json_len = strlen(json);
    char *buffer = (char *)cJSON_malloc(json_len + 1);
    memset(buffer, 0, json_len + 1);
    memcpy(buffer, json, json_len + 1);
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *my_array = NULL;
    cJSON *num_item0 = NULL;
    cJSON *num_item1 = NULL;
    cJSON *num_item2 = NULL;
    cJSON *replacement = NULL;
    cJSON *value_item = NULL;
    cJSON *value_added = NULL;
    cJSON *arr_verify_item = NULL;
    char *printed = NULL;
    double extracted_value = 0.0;
    double verify_value = 0.0;

    // step 2: Setup / Configure
    parsed = cJSON_Parse(buffer);
    root = cJSON_CreateObject();
    my_array = cJSON_CreateArray();
    num_item0 = cJSON_CreateNumber(1.0);
    num_item1 = cJSON_CreateNumber(2.0);
    num_item2 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(my_array, num_item0);
    cJSON_AddItemToArray(my_array, num_item1);
    cJSON_AddItemToArray(my_array, num_item2);
    cJSON_AddItemToObject(root, "modified", my_array);
    value_item = cJSON_GetObjectItem(parsed, "value");
    extracted_value = cJSON_GetNumberValue(value_item);
    value_added = cJSON_CreateNumber(extracted_value + 0.5);
    cJSON_AddItemToObject(root, "value_plus", value_added);

    // step 3: Operate / Validate
    replacement = cJSON_CreateNumber(7.7);
    cJSON_ReplaceItemInArray(my_array, 1, replacement);
    arr_verify_item = cJSON_GetArrayItem(my_array, 1);
    verify_value = cJSON_GetNumberValue(arr_verify_item);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
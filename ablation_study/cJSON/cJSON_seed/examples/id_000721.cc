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
//<ID> 721
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
    cJSON *container = cJSON_CreateArray();
    double numbers[3] = {1.5, 2.5, 3.5};
    cJSON *num_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON *str_item = cJSON_CreateString("example_string");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *temp_item = cJSON_CreateString("to_be_detached");

    // step 2: Configure
    cJSON_AddItemToArray(container, num_array);
    cJSON_AddItemToArray(container, str_item);
    cJSON_AddItemToArray(container, false_item);
    cJSON_AddItemToObject(root, "data", container);
    cJSON_AddItemToObject(root, "temp", temp_item);

    // Detach the "temp" item and move it into the array
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, "temp");
    cJSON_AddItemToArray(container, detached);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_data = cJSON_GetObjectItemCaseSensitive(parsed, "data");
    cJSON_bool parsed_is_array = cJSON_IsArray(parsed_data);
    cJSON *parsed_num_array = cJSON_GetArrayItem(parsed_data, 0);
    cJSON *parsed_first_num = cJSON_GetArrayItem(parsed_num_array, 0);
    double first_num = cJSON_GetNumberValue(parsed_first_num);
    cJSON *parsed_second = cJSON_GetArrayItem(parsed_data, 1);
    const char *second_str = cJSON_GetStringValue(parsed_second);
    cJSON *parsed_third = cJSON_GetArrayItem(parsed_data, 2);
    cJSON_bool third_is_false = cJSON_IsFalse(parsed_third);
    cJSON *parsed_fourth = cJSON_GetArrayItem(parsed_data, 3);
    const char *fourth_str = cJSON_GetStringValue(parsed_fourth);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    (void)version;
    (void)parsed_is_array;
    (void)first_num;
    (void)second_str;
    (void)third_is_false;
    (void)fourth_str;
    // API sequence test completed successfully
    return 66;
}
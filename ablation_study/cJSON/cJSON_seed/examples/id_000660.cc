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
//<ID> 660
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
    int values[4] = {10, 20, 30, 40};
    cJSON *int_array = cJSON_CreateIntArray(values, 4);
    cJSON *bool_node = cJSON_CreateBool(1);
    cJSON *dup_array = cJSON_Duplicate(int_array, 1);

    // step 2: Configure
    cJSON_AddItemToObject(root, "numbers", int_array);
    cJSON_AddItemToObjectCS(root, "NumbersCopy", dup_array);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "enabled", bool_node);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_numbers = cJSON_GetObjectItem(parsed, "numbers");
    int parsed_size = cJSON_GetArraySize(parsed_numbers);
    cJSON *first = cJSON_GetArrayItem(parsed_numbers, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *parsed_copy = cJSON_GetObjectItem(parsed, "NumbersCopy");
    cJSON_bool arrays_equal = cJSON_Compare(parsed_numbers, parsed_copy, 1);
    cJSON_bool parsed_is_array = cJSON_IsArray(parsed_numbers);
    (void)version;
    (void)parsed_size;
    (void)first_val;
    (void)arrays_equal;
    (void)parsed_is_array;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
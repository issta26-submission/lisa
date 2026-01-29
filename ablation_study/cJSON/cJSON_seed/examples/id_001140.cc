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
//<ID> 1140
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"numbers\":[10,20,30],\"meta\":{\"name\":\"sample\"}}";
    size_t json_len = strlen(json_text);
    cJSON *root = cJSON_ParseWithLength(json_text, json_len);
    cJSON *root_copy = cJSON_Parse(json_text);
    cJSON *null_item = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToObject(root, "nullable", null_item);
    cJSON_AddItemToObject(root, "original_copy", root_copy);

    // step 3: Operate & Validate
    cJSON *numbers = cJSON_GetObjectItem(root, "numbers");
    cJSON *second_elem = cJSON_GetArrayItem(numbers, 1);
    double second_value = cJSON_GetNumberValue(second_elem);
    cJSON *first_elem = cJSON_GetArrayItem(numbers, 0);
    double first_value = cJSON_GetNumberValue(first_elem);
    cJSON *sum_item = cJSON_CreateNumber(first_value + second_value);
    cJSON_AddItemToObject(root, "sum_first_second", sum_item);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}
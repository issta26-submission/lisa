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
//<ID> 834
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    cJSON *root = cJSON_CreateObject();
    double nums[3] = { 1.1, 2.2, 3.3 };
    cJSON *darr = cJSON_CreateDoubleArray(nums, 3);
    cJSON *raw = cJSON_CreateRaw("{\"raw\":true}");
    cJSON_AddItemToObjectCS(root, "values", darr);
    cJSON_AddItemToObjectCS(root, "raw_entry", raw);

    // step 2: Operate
    cJSON *replacement = cJSON_CreateNumber(9.9);
    cJSON_ReplaceItemInArray(darr, 1, replacement);
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_values = cJSON_GetObjectItem(parsed, "values");
    cJSON *parsed_item1 = cJSON_GetArrayItem(parsed_values, 1);
    double extracted = cJSON_GetNumberValue(parsed_item1);
    cJSON *adjusted = cJSON_CreateNumber(extracted + 0.1);
    cJSON_AddItemToObjectCS(parsed, "adjusted", adjusted);

    // step 3: Validate
    cJSON_bool is_num = cJSON_IsNumber(parsed_item1);
    cJSON *flag = cJSON_CreateBool(is_num);
    cJSON_AddItemToObjectCS(parsed, "is_number", flag);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
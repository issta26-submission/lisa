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
//<ID> 1363
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"numbers\":[1,2,3.5],\"meta\":{\"count\":3}}";
    size_t json_len = strlen(json);
    cJSON *parsed = cJSON_ParseWithLength(json, json_len);
    float nums[3] = {4.5f, 5.5f, 6.5f};
    cJSON *float_arr = cJSON_CreateFloatArray(nums, 3);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON_AddItemToObject(root, "float_values", float_arr);
    cJSON *numbers = cJSON_GetObjectItem(parsed, "numbers");
    cJSON *numbers_ref = cJSON_CreateArrayReference(numbers);
    cJSON_AddItemToObject(root, "numbers_ref", numbers_ref);

    // step 3: Operate
    cJSON *first = cJSON_GetArrayItem(numbers, 0);
    cJSON_bool first_is_num = cJSON_IsNumber(first);
    cJSON *isnum_item = cJSON_CreateBool(first_is_num);
    cJSON_AddItemToObject(root, "first_is_number", isnum_item);
    cJSON *float_dup = cJSON_Duplicate(float_arr, 1);
    cJSON_AddItemToObject(root, "float_dup", float_dup);

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
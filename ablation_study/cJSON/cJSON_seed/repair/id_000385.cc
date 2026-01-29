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
//<ID> 385
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"numbers\":[1,2,3], \"flag\":true}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *version_item = cJSON_CreateString(version);
    cJSON_AddItemToObject(root, "version", version_item);
    cJSON_bool has_nums = cJSON_HasObjectItem(parsed, "numbers");
    cJSON *has_numbers_item = cJSON_CreateBool(has_nums);
    cJSON_AddItemToObject(root, "has_numbers", has_numbers_item);
    cJSON *numbers = cJSON_GetObjectItem(parsed, "numbers");
    cJSON *merged = cJSON_CreateArray();
    cJSON *n0 = cJSON_GetArrayItem(numbers, 0);
    cJSON *n1 = cJSON_GetArrayItem(numbers, 1);
    cJSON *dup0 = cJSON_Duplicate(n0, 1);
    cJSON *dup1 = cJSON_Duplicate(n1, 1);
    cJSON_AddItemToArray(merged, dup0);
    cJSON_AddItemToArray(merged, dup1);
    cJSON *extra_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(merged, extra_num);
    cJSON_AddItemToObject(root, "merged", merged);

    // step 3: Operate and Validate
    double v0 = cJSON_GetNumberValue(n0);
    double v1_val = cJSON_GetNumberValue(n1);
    double total = v0 + v1_val;
    cJSON *sum_item = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "sum", sum_item);
    cJSON *flag_item = cJSON_GetObjectItem(parsed, "flag");
    cJSON_bool flag_is_bool = cJSON_IsBool(flag_item);
    cJSON *flag_copy = cJSON_CreateBool(flag_is_bool && cJSON_IsTrue(flag_item));
    cJSON_AddItemToObject(root, "flag_true", flag_copy);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 718
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int nums[] = {10, 20, 30};
    cJSON *int_arr = cJSON_CreateIntArray(nums, 3);
    cJSON *meta_str = cJSON_CreateString("metadata");
    const char *json_text = "{\"numbers\":[1,2,3],\"name\":\"alpha\"}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json_text, (size_t)128, &parse_end, 1);
    (void)parse_end;

    // step 2: Configure
    cJSON_AddItemToObject(root, "ints", int_arr);
    cJSON_AddItemToObject(root, "meta", meta_str);
    cJSON *ints_count_num = cJSON_CreateNumber((double)cJSON_GetArraySize(int_arr));
    cJSON_AddItemToObject(root, "ints_count", ints_count_num);

    // step 3: Operate
    cJSON *parsed_numbers = cJSON_GetObjectItem(root, "numbers");
    cJSON *parsed_numbers_count_num = cJSON_CreateNumber((double)cJSON_GetArraySize(parsed_numbers));
    cJSON_AddItemToObject(root, "parsed_numbers_count", parsed_numbers_count_num);
    const char *orig_name = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    cJSON *name_copy = cJSON_CreateString(orig_name);
    cJSON_AddItemToObject(root, "name_copy", name_copy);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
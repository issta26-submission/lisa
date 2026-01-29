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
//<ID> 717
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"numbers\":[10,20,30],\"title\":\"orig\"}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, (size_t)(sizeof(json) - 1), &parse_end, 1);

    // step 2: Configure
    cJSON *numbers = cJSON_GetObjectItem(root, "numbers");
    int orig_count = cJSON_GetArraySize(numbers);
    cJSON *orig_count_item = cJSON_CreateNumber((double)orig_count);
    cJSON_AddItemToObject(root, "orig_count", orig_count_item);
    const char *title_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "title"));
    cJSON *title_copy = cJSON_CreateString(title_str);
    cJSON_AddItemToObject(root, "title_copy", title_copy);

    // step 3: Operate
    int extra_nums[] = {1, 2, 3, 4};
    cJSON *int_arr = cJSON_CreateIntArray(extra_nums, 4);
    cJSON_AddItemToObject(root, "int_arr", int_arr);
    int int_arr_size = cJSON_GetArraySize(int_arr);
    cJSON *int_arr_size_item = cJSON_CreateNumber((double)int_arr_size);
    cJSON_AddItemToObject(root, "int_arr_size", int_arr_size_item);

    // step 4: Validate & Cleanup
    cJSON *root_dup = cJSON_Duplicate(root, 1);
    char *out = cJSON_PrintUnformatted(root_dup);
    cJSON_free(out);
    cJSON_Delete(root_dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
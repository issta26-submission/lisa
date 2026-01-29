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
//<ID> 1218
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"nums\":[10,20,30],\"remove_me\":7.5}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *new_arr = cJSON_CreateArray();

    // step 2: Configure
    cJSON *parsed_nums = cJSON_GetObjectItem(parsed, "nums");
    cJSON *arr_ref = cJSON_CreateArrayReference(parsed_nums);
    cJSON_AddItemToObject(root, "nums_ref", arr_ref);
    cJSON *remove_item = cJSON_GetObjectItem(parsed, "remove_me");
    double removed_val = cJSON_GetNumberValue(remove_item);
    cJSON_AddNumberToObject(root, "migrated_remove_me", removed_val);
    cJSON_AddItemToObject(root, "copied_arr", new_arr);
    cJSON *n0 = cJSON_CreateNumber(99.0);
    cJSON_AddItemToArray(new_arr, n0);

    // step 3: Operate & Validate
    cJSON_DeleteItemFromObject(parsed, "remove_me");
    cJSON *first_in_parsed = cJSON_GetArrayItem(parsed_nums, 0);
    double first_val = cJSON_GetNumberValue(first_in_parsed);
    cJSON_AddNumberToObject(root, "first_val", first_val);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}
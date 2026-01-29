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
//<ID> 1220
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"nums\":[1,2,3,4],\"name\":\"example\",\"flag\":1}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    int manual_numbers[4] = {10, 20, 30, 40};
    cJSON *manual_array = cJSON_CreateIntArray(manual_numbers, 4);
    cJSON_AddItemToObject(root, "manual_ints", manual_array);
    cJSON *nums = cJSON_GetObjectItem(parsed, "nums");

    // step 3: Operate & Validate
    cJSON *first_item = cJSON_DetachItemFromArray(nums, 0);
    cJSON_AddItemToObject(root, "first_from_parsed", first_item);
    cJSON_bool first_is_number = cJSON_IsNumber(first_item);
    cJSON_AddBoolToObject(root, "first_is_number", first_is_number);
    double first_value = cJSON_GetNumberValue(first_item);
    (void)first_value; // value used to show meaningful data flow without branches

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}
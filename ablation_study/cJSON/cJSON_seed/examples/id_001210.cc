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
//<ID> 1210
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"values\":[10,20,30],\"remove_me\":123.45}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *values_arr = cJSON_GetObjectItem(parsed, "values");
    cJSON *arr_ref = cJSON_CreateArrayReference(values_arr);
    cJSON_AddItemToObject(root, "referenced_values", arr_ref);
    cJSON *rem_item = cJSON_GetObjectItem(parsed, "remove_me");
    double rem_value = cJSON_GetNumberValue(rem_item);

    // step 3: Operate & Validate
    cJSON_DeleteItemFromObject(parsed, "remove_me");
    cJSON_AddNumberToObject(root, "migrated_number", rem_value);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}
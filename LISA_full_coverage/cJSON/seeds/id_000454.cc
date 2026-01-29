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
//<ID> 454
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"numbers\":[1.1,2.2,3.3],\"flag\":false}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *numbers = cJSON_GetObjectItem(root, "numbers");
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(numbers, 0));
    float float_vals[2];
    float_vals[0] = (float)first_val;
    float_vals[1] = 9.9f;

    // step 2: Configure
    cJSON *float_arr = cJSON_CreateFloatArray(float_vals, 2);
    cJSON_AddItemToObject(root, "float_copy", float_arr);
    cJSON *new_bool = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "new_flag", new_bool);

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromArray(numbers, 1);
    cJSON_AddItemToArray(float_arr, detached);
    int remaining = cJSON_GetArraySize(numbers);
    (void)remaining;
    cJSON_bool was_bool = cJSON_IsBool(flag_item);
    (void)was_bool;

    // step 4: Validate & Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}
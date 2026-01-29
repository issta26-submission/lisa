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
//<ID> 451
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"values\":[1.1,2.2,3.3],\"flag\":false}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *values = cJSON_GetObjectItem(root, "values");
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");

    // step 2: Configure
    float extra_vals[2];
    extra_vals[0] = 4.4f;
    extra_vals[1] = 5.5f;
    cJSON *more_values = cJSON_CreateFloatArray(extra_vals, 2);
    cJSON_AddItemToObject(root, "more_values", more_values);
    cJSON *flag_copy = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "flag_copy", flag_copy);
    char empty_buf[16];
    memset(empty_buf, 0, sizeof(empty_buf));
    cJSON *empty_str = cJSON_CreateString(empty_buf);
    cJSON_AddItemToObject(root, "empty", empty_str);

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromArray(values, 1);
    cJSON_AddItemToArray(more_values, detached);
    int original_count = cJSON_GetArraySize(values);
    int more_count = cJSON_GetArraySize(more_values);
    cJSON *first_more = cJSON_GetArrayItem(more_values, 0);
    double first_value = cJSON_GetNumberValue(first_more);
    (void)original_count;
    (void)more_count;
    (void)first_value;
    (void)flag_item;

    // step 4: Validate & Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}
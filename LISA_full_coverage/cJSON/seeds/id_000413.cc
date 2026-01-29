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
//<ID> 413
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"arr\":[1,2,3],\"obj\":{\"key\":\"val\"}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, sizeof(json) - 1, &parse_end, 1);

    // step 2: Configure
    cJSON *arr = cJSON_GetObjectItemCaseSensitive(root, "arr");
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *created_object = cJSON_CreateObject();

    // step 3: Operate
    cJSON *scaled_number = cJSON_CreateNumber(second_val * 10.0);
    cJSON_AddItemToObject(created_object, "scaled", scaled_number);
    cJSON_InsertItemInArray(arr, 1, created_object);
    cJSON *item_at_one = cJSON_GetArrayItem(arr, 1);

    // step 4: Validate and Cleanup
    cJSON *scaled_in_obj = cJSON_GetObjectItemCaseSensitive(item_at_one, "scaled");
    double validated = cJSON_GetNumberValue(scaled_in_obj);
    (void)validated;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
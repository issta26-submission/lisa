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
//<ID> 452
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"values\":[10.0,20.0,30.0],\"flags\":[false,true]}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *values = cJSON_GetObjectItem(root, "values");
    cJSON *flags = cJSON_GetObjectItem(root, "flags");

    // step 2: Configure
    float new_numbers[3];
    new_numbers[0] = 1.1f;
    new_numbers[1] = 2.2f;
    new_numbers[2] = 3.3f;
    cJSON *float_arr = cJSON_CreateFloatArray(new_numbers, 3);
    cJSON_AddItemToObject(root, "float_arr", float_arr);
    cJSON *added_bool = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "added_bool", added_bool);

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromArray(values, 1);
    cJSON_AddItemToArray(float_arr, detached);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}
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
//<ID> 450
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"meta\":\"test\"}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);

    // step 2: Configure
    float numbers[3];
    numbers[0] = 1.5f;
    numbers[1] = 2.5f;
    numbers[2] = 3.5f;
    cJSON *float_arr = cJSON_CreateFloatArray(numbers, 3);
    cJSON_AddItemToObject(root, "floats", float_arr);
    cJSON_bool active_flag = 1;
    cJSON *active_item = cJSON_CreateBool(active_flag);
    cJSON_AddItemToObject(root, "active", active_item);

    // step 3: Operate
    cJSON *retrieved = cJSON_GetObjectItem(root, "floats");
    cJSON *detached = cJSON_DetachItemFromArray(retrieved, 1);
    cJSON *inserted_flag = cJSON_CreateBool(0);
    cJSON_AddItemToArray(retrieved, inserted_flag);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}
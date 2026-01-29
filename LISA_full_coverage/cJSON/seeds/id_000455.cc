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
//<ID> 455
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"root\": {\"numbers\": [1.1, 2.2, 3.3], \"flag\": false}}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *root_obj = cJSON_GetObjectItem(root, "root");
    cJSON *numbers = cJSON_GetObjectItem(root_obj, "numbers");
    cJSON *flag_item = cJSON_GetObjectItem(root_obj, "flag");

    // step 2: Configure
    float new_vals[3];
    new_vals[0] = 4.4f;
    new_vals[1] = 5.5f;
    new_vals[2] = 6.6f;
    cJSON *float_array = cJSON_CreateFloatArray(new_vals, 3);
    cJSON *bool_item = cJSON_CreateBool(1);
    char label_buf[32];
    memset(label_buf, 0, sizeof(label_buf));
    label_buf[0] = 'A';
    cJSON *label_str = cJSON_CreateString(label_buf);

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromArray(numbers, 0);
    cJSON_AddItemToArray(float_array, detached);
    cJSON_AddItemToObject(root_obj, "new_floats", float_array);
    cJSON_AddItemToObject(root_obj, "active", bool_item);
    cJSON_AddItemToObject(root_obj, "label", label_str);
    int combined_size = cJSON_GetArraySize(float_array);
    (void)combined_size;
    cJSON_bool flag_is_true = cJSON_IsTrue(flag_item);
    (void)flag_is_true;
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}
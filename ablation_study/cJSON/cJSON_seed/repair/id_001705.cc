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
//<ID> 1705
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(root, "count", 7.0);
    float floats_arr_values[3];
    floats_arr_values[0] = 1.0f;
    floats_arr_values[1] = 2.5f;
    floats_arr_values[2] = -3.25f;
    cJSON *floats_arr = cJSON_CreateFloatArray(floats_arr_values, 3);
    cJSON_AddItemToObject(root, "floats", floats_arr);
    cJSON *active_bool = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "active", active_bool);
    cJSON_AddFalseToObject(root, "is_disabled");

    // step 2: Configure
    cJSON_AddItemToObject(meta, "name", cJSON_CreateString("example_meta"));
    cJSON_AddNumberToObject(meta, "threshold", 0.75);

    // step 3: Operate and Validate
    cJSON *retr_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *retr_name_item = cJSON_GetObjectItem(retr_meta, "name");
    const char *name_val = cJSON_GetStringValue(retr_name_item);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_val ? name_val[0] : 'Y';
    buffer[2] = (char)('0' + (int)cJSON_GetArraySize(floats_arr));
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}
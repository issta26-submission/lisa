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
//<ID> 1702
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddNumberToObject(root, "version", 1.23);
    cJSON_AddFalseToObject(root, "available");
    cJSON *enabled_bool = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", enabled_bool);
    float values[3] = {0.1f, 2.5f, 3.75f};
    cJSON *float_array = cJSON_CreateFloatArray(values, 3);
    cJSON_AddItemToObject(root, "values", float_array);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "count", 3.0);
    cJSON_AddFalseToObject(meta, "initialized");

    // step 3: Operate and Validate
    cJSON *retr_name = cJSON_GetObjectItem(root, "name");
    char *name_val = cJSON_GetStringValue(retr_name);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = name_val ? name_val[0] : 'X';
    buffer[1] = printed ? printed[0] : 'Y';
    buffer[2] = (char)('0' + (int)cJSON_GetArraySize(float_array));
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}
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
//<ID> 1707
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
    cJSON *name = cJSON_CreateString("cjson_test");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON_AddNumberToObject(root, "version", 1.23);
    cJSON *active_false = cJSON_AddFalseToObject(root, "active");
    cJSON *enabled_bool = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", enabled_bool);
    float nums[3] = { 1.1f, 2.2f, 3.3f };
    cJSON *float_array = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(root, "floats", float_array);

    // step 2: Configure
    cJSON *retr_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *retr_name = cJSON_GetObjectItem(retr_meta, "name");
    const char *name_val = cJSON_GetStringValue(retr_name);
    cJSON *retr_version = cJSON_GetObjectItem(root, "version");
    double version_val = cJSON_GetNumberValue(retr_version);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed[0];
    buffer[1] = name_val[0];
    buffer[2] = (char)('0' + (int)version_val);
    buffer[3] = (char)('0' + (int)cJSON_IsFalse(active_false));
    (void)buffer;
    (void)float_array;
    (void)enabled_bool;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}
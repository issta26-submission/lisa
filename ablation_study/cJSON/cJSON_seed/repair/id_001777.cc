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
//<ID> 1777
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON *name_item = cJSON_CreateString("device42");
    cJSON_AddItemToObject(settings, "name", name_item);
    cJSON *threshold_item = cJSON_CreateNumber(4.2);
    cJSON_AddItemToObject(settings, "threshold", threshold_item);
    cJSON *enabled_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(settings, "enabled", enabled_item);

    // step 2: Configure
    cJSON *params = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "params", params);
    cJSON *p0 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(params, p0);
    cJSON *p1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(params, p1);
    cJSON *p2 = cJSON_CreateString("fast");
    cJSON_AddItemToArray(params, p2);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "version", cJSON_Version());

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    const cJSON *got_name = cJSON_GetObjectItem(settings, "name");
    cJSON_bool name_is_str = cJSON_IsString(got_name);
    const char *name_str = cJSON_GetStringValue(got_name);
    const cJSON *got_threshold = cJSON_GetObjectItem(settings, "threshold");
    double threshold_val = cJSON_GetNumberValue(got_threshold);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_str ? name_str[0] : 'Z';
    buffer[2] = (char)('0' + ((int)threshold_val % 10));
    (void)name_is_str;
    (void)threshold_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}
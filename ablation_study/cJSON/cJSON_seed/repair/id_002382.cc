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
//<ID> 2382
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
    cJSON *num_item = cJSON_CreateNumber(123.5);

    // step 2: Configure
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON_AddItemToObject(settings, "value", num_item);
    cJSON *enabled_item = cJSON_AddTrueToObject(settings, "enabled");
    cJSON *extra_num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "extra", extra_num);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool has_settings = cJSON_HasObjectItem(root, "settings");
    cJSON *found_settings = cJSON_GetObjectItem(root, "settings");
    cJSON *found_value = cJSON_GetObjectItem(found_settings, "value");
    double value = cJSON_GetNumberValue(found_value);
    buffer[0] = (printed && printed[0]) ? printed[0] : '\0';
    buffer[1] = (char)(int)value;
    buffer[2] = (char)(int)has_settings;
    (void)enabled_item; // use to avoid unused-variable warnings in some builds

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
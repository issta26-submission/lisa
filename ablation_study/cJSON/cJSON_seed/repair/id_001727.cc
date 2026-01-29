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
//<ID> 1727
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", enabled);
    cJSON *threshold = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(config, "threshold", threshold);
    cJSON *name = cJSON_CreateString("sensor");
    cJSON_AddItemToObject(config, "name", name);

    // step 2: Configure
    cJSON_AddNullToObject(config, "optional");
    double new_threshold = cJSON_SetNumberHelper(threshold, 42.5);
    (void)new_threshold;

    // step 3: Operate and Validate
    cJSON *retr_config = cJSON_GetObjectItem(root, "config");
    cJSON *retr_enabled = cJSON_GetObjectItem(retr_config, "enabled");
    cJSON_bool is_enabled = cJSON_IsTrue(retr_enabled);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = is_enabled ? 'T' : 'F';
    const char *name_val = cJSON_GetStringValue(cJSON_GetObjectItem(retr_config, "name"));
    buffer[2] = name_val ? name_val[0] : 'Z';
    double thr_val = cJSON_GetNumberValue(cJSON_GetObjectItem(retr_config, "threshold"));
    buffer[3] = (char)('0' + ((int)thr_val % 10));
    (void)buffer;
    (void)thr_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66;
} // API sequence test completed successfully
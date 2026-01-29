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
//<ID> 2263
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
    cJSON *name = cJSON_CreateString("device42");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *count = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(config, "count", count);

    // step 2: Configure
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "values", values);
    cJSON *v0 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(values, v0);
    cJSON *v1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(values, v1);

    // step 3: Operate and Validate
    const char *version_str = cJSON_Version();
    cJSON *ver_item = cJSON_CreateString(version_str);
    cJSON_AddItemToObject(root, "version", ver_item);
    cJSON *config_dup = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_dup", config_dup);
    cJSON *new_name = cJSON_CreateString("device99");
    cJSON_ReplaceItemViaPointer(config, name, new_name);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = version_str ? version_str[0] : '\0';
    buffer[2] = (char)((int)cJSON_GetNumberValue(count) % 256);
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
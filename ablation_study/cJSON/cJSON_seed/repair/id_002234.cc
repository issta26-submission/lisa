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
//<ID> 2234
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);
    cJSON *name = cJSON_CreateString("device-alpha");
    cJSON_AddItemToObject(info, "name", name);
    cJSON *version = cJSON_CreateNumber(2.5);
    cJSON_AddItemToObject(info, "version", version);
    cJSON *active = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "active", active);

    // step 2: Configure
    cJSON *data = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *d0 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(data, d0);
    cJSON *d1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(data, d1);
    cJSON *label = cJSON_CreateString("payload");
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate and Validate
    cJSON *got_info = cJSON_GetObjectItem(root, "info");
    cJSON *got_version = cJSON_GetObjectItem(got_info, "version");
    double ver = cJSON_GetNumberValue(got_version);
    cJSON *ver_copy = cJSON_CreateNumber(ver);
    cJSON_AddItemToObject(info, "version_copy", ver_copy);
    cJSON *got_name = cJSON_GetObjectItem(got_info, "name");
    char *name_str = cJSON_GetStringValue(got_name);
    cJSON *name_copy = cJSON_CreateString(name_str ? name_str : "");
    cJSON_AddItemToObject(info, "name_copy", name_copy);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = (json && json[2]) ? json[2] : '\0';
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
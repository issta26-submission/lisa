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
//<ID> 2161
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
    cJSON *enabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(config, "enabled", enabled);
    cJSON *count = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "count", count);

    // step 2: Configure
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *it1 = cJSON_CreateString("one");
    cJSON_AddItemToArray(items, it1);
    cJSON *it2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(items, it2);

    // step 3: Operate and Validate
    cJSON *got_enabled = cJSON_GetObjectItemCaseSensitive(config, "enabled");
    cJSON_bool enabled_is_bool = cJSON_IsBool(got_enabled);
    cJSON *enabled_flag = cJSON_CreateBool(enabled_is_bool);
    cJSON_AddItemToObject(root, "enabled_is_bool", enabled_flag);
    cJSON *got_name = cJSON_GetObjectItemCaseSensitive(config, "name");
    cJSON_bool name_is_string = cJSON_IsString(got_name);
    cJSON *name_flag = cJSON_CreateBool(name_is_string);
    cJSON_AddItemToObject(root, "name_is_string", name_flag);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = json[0];
    buffer[1] = json[1];
    buffer[2] = json[2];
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(json);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
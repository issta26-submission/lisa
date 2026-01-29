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
//<ID> 2289
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
    cJSON *active_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(meta, "active", active_false);
    cJSON_AddNullToObject(meta, "deprecated");
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddStringToObject(data, "id", "sensor-007");
    cJSON_AddNumberToObject(data, "value", 123.0);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "count", 3.0);
    cJSON_AddStringToObject(root, "name", "device-1");
    cJSON *flags = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "flags", flags);

    // step 3: Operate
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_active = cJSON_GetObjectItem(got_meta, "active");
    cJSON *got_deprecated = cJSON_GetObjectItem(got_meta, "deprecated");
    cJSON_bool active_is_false = cJSON_IsFalse(got_active);
    cJSON_bool active_is_bool = cJSON_IsBool(got_active);
    cJSON_bool deprecated_is_null = cJSON_IsNull(got_deprecated);
    cJSON_AddItemToObject(flags, "active_is_false", cJSON_CreateBool(active_is_false));
    cJSON_AddItemToObject(flags, "active_is_bool", cJSON_CreateBool(active_is_bool));
    cJSON_AddItemToObject(flags, "deprecated_is_null", cJSON_CreateBool(deprecated_is_null));

    // step 4: Validate and Cleanup
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (active_is_false ? '1' : '0');
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
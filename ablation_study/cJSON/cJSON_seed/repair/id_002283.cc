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
//<ID> 2283
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
    cJSON *enabled_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta, "enabled", enabled_true);
    cJSON *optional_null = cJSON_AddNullToObject(meta, "optional");

    // step 2: Configure
    cJSON *count = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "count", count);
    cJSON *label = cJSON_CreateString("sensor-X");
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate
    cJSON *got_optional = cJSON_GetObjectItem(meta, "optional");
    cJSON_bool optional_is_null = cJSON_IsNull(got_optional);
    cJSON *got_enabled = cJSON_GetObjectItem(meta, "enabled");
    cJSON_bool enabled_is_bool = cJSON_IsBool(got_enabled);
    cJSON_bool enabled_is_false = cJSON_IsFalse(got_enabled);
    cJSON_AddNumberToObject(root, "optional_is_null", (double)optional_is_null);
    cJSON_AddNumberToObject(root, "enabled_is_bool", (double)enabled_is_bool);
    cJSON_AddNumberToObject(root, "enabled_is_false", (double)enabled_is_false);

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
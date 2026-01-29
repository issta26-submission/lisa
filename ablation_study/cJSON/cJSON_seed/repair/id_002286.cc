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
//<ID> 2286
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
    cJSON *null_node = cJSON_AddNullToObject(meta, "optional");
    cJSON *enabled_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(meta, "enabled", enabled_false);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "count", 7.0);
    cJSON *details = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "details", details);
    cJSON_AddStringToObject(details, "id", "dev-01");
    cJSON *ok_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(details, "ok", ok_true);

    // step 3: Operate
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_optional = cJSON_GetObjectItem(got_meta, "optional");
    cJSON_bool optional_is_null = cJSON_IsNull(got_optional);
    cJSON *opt_null_num = cJSON_CreateNumber((double)optional_is_null);
    cJSON_AddItemToObject(root, "optional_is_null", opt_null_num);
    cJSON_bool optional_is_bool = cJSON_IsBool(got_optional);
    cJSON *opt_is_bool_num = cJSON_CreateNumber((double)optional_is_bool);
    cJSON_AddItemToObject(root, "optional_is_bool", opt_is_bool_num);
    cJSON_bool enabled_is_false = cJSON_IsFalse(cJSON_GetObjectItem(got_meta, "enabled"));
    cJSON *enabled_false_num = cJSON_CreateNumber((double)enabled_is_false);
    cJSON_AddItemToObject(root, "enabled_is_false", enabled_false_num);

    // step 4: Validate & Cleanup
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    (void)printed;
    cJSON_free(buffer);
    cJSON_free(json);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
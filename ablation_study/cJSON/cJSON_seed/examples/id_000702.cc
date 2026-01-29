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
//<ID> 702
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *str_item = cJSON_CreateString("sensor");
    cJSON_AddItemToArray(values, num_item);
    cJSON_AddItemToArray(values, str_item);
    cJSON_AddItemToObject(root, "values", values);
    cJSON *meta = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("deviceA");
    cJSON_AddItemToObject(meta, "name", name_item);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *extra_str = cJSON_CreateString("locationX");
    cJSON_AddItemToObject(root, "location", extra_str);
    (void)extra_str;

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_meta = cJSON_GetObjectItemCaseSensitive(parsed, "meta");
    cJSON *parsed_name = cJSON_GetObjectItemCaseSensitive(parsed_meta, "name");
    const char *name_value = cJSON_GetStringValue(parsed_name);
    cJSON *parsed_values = cJSON_GetObjectItemCaseSensitive(parsed, "values");
    (void)name_value;
    (void)parsed_values;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
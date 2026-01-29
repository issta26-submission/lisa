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
//<ID> 914
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"root\":{\"values\":[10,20]},\"meta\":\"detachable\"}";
    size_t json_len = strlen(json);
    cJSON *doc = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    cJSON *root = cJSON_GetObjectItemCaseSensitive(doc, "root");
    cJSON *values = cJSON_GetObjectItemCaseSensitive(root, "values");
    cJSON *extras = cJSON_AddArrayToObject(root, "extras");
    cJSON *num_node = cJSON_CreateNumber(42.0);
    cJSON *str_node = cJSON_CreateString("hello");
    cJSON_AddItemToArray(extras, num_node);
    cJSON_AddItemToArray(extras, str_node);
    cJSON *detached_meta = cJSON_DetachItemFromObject(doc, "meta");
    cJSON_AddItemToArray(extras, detached_meta);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(doc);
    cJSON *extras_lookup = cJSON_GetObjectItemCaseSensitive(root, "extras");
    cJSON *first_value = cJSON_GetArrayItem(values, 0);
    double first_num = cJSON_GetNumberValue(first_value);
    cJSON *incremented = cJSON_CreateNumber(first_num + 1.0);
    cJSON_AddItemToArray(extras_lookup, incremented);

    // step 4: Validate & Cleanup
    void *buffer = cJSON_malloc(256);
    memset(buffer, 0, 256);
    char *prebuf = (char *)buffer;
    cJSON_PrintPreallocated(doc, prebuf, 256, 1);
    cJSON_free(prebuf);
    cJSON_free(out);
    cJSON_Delete(doc);

    // API sequence test completed successfully
    return 66;
}
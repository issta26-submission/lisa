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
//<ID> 988
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *values = NULL;
    cJSON *second_item = NULL;
    cJSON *dup_values = NULL;
    cJSON *false_node = NULL;
    char *minified = NULL;
    char *out = NULL;
    const char json_src[] = "{ \"device\":\"therm\",\"values\" : [ 10, 20, 30 ], \"active\": true }";
    size_t src_len = (size_t)(sizeof(json_src) - 1);
    size_t buf_len = src_len + 1;
    double second_value = 0.0;

    // step 2: Setup / Configure
    minified = (char *)cJSON_malloc(buf_len);
    memset(minified, 0, buf_len);
    memcpy(minified, json_src, src_len);
    minified[src_len] = '\0';
    cJSON_Minify(minified);
    parsed = cJSON_Parse(minified);
    values = cJSON_GetObjectItem(parsed, "values");
    second_item = cJSON_GetArrayItem(values, 1);
    second_value = cJSON_GetNumberValue(second_item);
    root = cJSON_CreateObject();
    dup_values = cJSON_Duplicate(values, 1);
    cJSON_AddItemToObject(root, "snapshot", dup_values);
    false_node = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "active_override", false_node);

    // step 3: Operate / Validate
    out = cJSON_PrintUnformatted(root);
    minified[0] = (char)('0' + ((int)second_value % 10));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(minified);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 228
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"value\": 42.5, \"info\": \"sample\"}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = cJSON_ParseWithLength(json, json_len);
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *name_node = cJSON_AddStringToObject(root, "name", "cjson_test");

    // step 2: Configure and Operate
    cJSON *retrieved_parsed = cJSON_GetObjectItem(root, "parsed");
    cJSON *num_item = cJSON_GetObjectItem(retrieved_parsed, "value");
    double number = cJSON_GetNumberValue(num_item);
    cJSON_AddNumberToObject(root, "value_doubled", number * 2.0);

    // step 3: Validate
    cJSON *validated = cJSON_GetObjectItem(root, "value_doubled");
    double confirmed = cJSON_GetNumberValue(validated);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
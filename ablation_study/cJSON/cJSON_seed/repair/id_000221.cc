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
//<ID> 221
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"example\",\"value\":123.45}";
    size_t json_len = (size_t)strlen(json);
    cJSON *parsed = cJSON_ParseWithLength(json, json_len);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *value_item = cJSON_GetObjectItem(parsed, "value");
    double numeric_value = cJSON_GetNumberValue(value_item);
    char buf[64];
    snprintf(buf, sizeof(buf), "%.2f", numeric_value);
    cJSON *value_str_item = cJSON_AddStringToObject(root, "value_str", buf);

    // step 3: Operate and Validate
    cJSON *retrieved = cJSON_GetObjectItem(root, "value_str");
    size_t value_length = strlen(retrieved->valuestring);
    cJSON_AddNumberToObject(root, "value_len", (double)value_length);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 224
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"alpha\",\"value\":99.5}";
    size_t json_len = sizeof(json) - 1;
    cJSON *parsed = cJSON_ParseWithLength(json, json_len);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *status = cJSON_AddStringToObject(root, "status", "ok");

    // step 3: Operate and Validate
    cJSON *retrieved = cJSON_GetObjectItem(root, "parsed");
    cJSON *name_item = cJSON_GetObjectItem(retrieved, "name");
    cJSON *value_item = cJSON_GetObjectItem(retrieved, "value");
    const char *name_str = cJSON_GetStringValue(name_item);
    double extracted_value = cJSON_GetNumberValue(value_item);
    cJSON_AddStringToObject(root, "extracted_name", name_str);
    cJSON_AddNumberToObject(root, "extracted_value", extracted_value);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
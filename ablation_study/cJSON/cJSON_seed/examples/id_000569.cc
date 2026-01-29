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
//<ID> 569
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *child = cJSON_CreateObject();
    const char *raw_text = "{\"raw_key\": 123}";
    cJSON *raw = cJSON_CreateRaw(raw_text);
    cJSON_AddItemToObject(child, "embedded", raw);
    cJSON_AddItemReferenceToObject(root, "child_ref", child);
    cJSON_AddStringToObject(root, "name", "example");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_child = cJSON_GetObjectItem(parsed, "child_ref");
    cJSON *parsed_embedded = cJSON_GetObjectItem(parsed_child, "embedded");
    cJSON *parsed_raw_key = cJSON_GetObjectItem(parsed_embedded, "raw_key");
    double raw_val = cJSON_GetNumberValue(parsed_raw_key);
    cJSON *parsed_name_item = cJSON_GetObjectItem(parsed, "name");
    char *name_str = (char*)cJSON_GetStringValue(parsed_name_item);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;raw=%.0f;name=%s",
            version ? version : "null",
            raw_val,
            name_str ? name_str : "null");
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
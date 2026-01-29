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
//<ID> 565
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
    cJSON *child = cJSON_CreateObject();
    cJSON *raw = cJSON_CreateRaw("{\"nested\":123}");

    // step 2: Configure
    cJSON_AddNumberToObject(child, "value", 7.5);
    cJSON_AddItemReferenceToObject(root, "child_ref", child);
    cJSON_AddItemReferenceToObject(root, "raw_ref", raw);
    cJSON_AddStringToObject(root, "label", "root_example");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_raw = cJSON_GetObjectItem(parsed, "raw_ref");
    char *parsed_raw_str = cJSON_GetStringValue(parsed_raw);
    cJSON *parsed_child = cJSON_GetObjectItem(parsed, "child_ref");
    cJSON *child_value_item = cJSON_GetObjectItem(parsed_child, "value");
    double child_value = cJSON_GetNumberValue(child_value_item);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;raw=%s;childv=%.2f",
            version ? version : "null",
            parsed_raw_str ? parsed_raw_str : "null",
            child_value);
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_Delete(child);
    cJSON_Delete(raw);
    // API sequence test completed successfully
    return 66;
}
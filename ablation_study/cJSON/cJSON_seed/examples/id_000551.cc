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
//<ID> 551
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
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", num);
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "missing", null_item);
    cJSON_AddStringToObject(root, "label", "test-label");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_label_item = cJSON_GetObjectItem(parsed, "label");
    const char *label_value = cJSON_GetStringValue(parsed_label_item);
    cJSON_AddStringToObject(root, "label_copy", label_value ? label_value : ""); 
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;label=%s", version ? version : "null", label_value ? label_value : "null");

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
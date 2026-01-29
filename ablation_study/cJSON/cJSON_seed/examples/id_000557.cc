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
//<ID> 557
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(root, "value", num);
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON *nul = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "optional", nul);
    cJSON_AddStringToObject(root, "label", "sensorA");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_label = cJSON_GetObjectItem(parsed, "label");
    char *label_val = (char *)cJSON_GetStringValue(parsed_label);
    cJSON *parsed_value = cJSON_GetObjectItem(parsed, "value");
    double parsed_value_val = cJSON_GetNumberValue(parsed_value);
    cJSON *new_num = cJSON_CreateNumber(100.0);
    cJSON_ReplaceItemInObject(root, "value", new_num);
    cJSON *repl = cJSON_GetObjectItem(root, "value");
    double repl_val = cJSON_GetNumberValue(repl);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "label=%s;orig=%.2f;parsed=%.2f;repl=%.2f",
            label_val ? label_val : "null",
            42.5,
            parsed_value_val,
            repl_val);
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
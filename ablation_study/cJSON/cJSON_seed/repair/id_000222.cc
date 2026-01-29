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
//<ID> 222
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"value\":123.5,\"label\":\"ok\"}";
    size_t json_len = sizeof(json) - 1;
    cJSON *parsed = cJSON_ParseWithLength(json, json_len);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *val_item = cJSON_GetObjectItem(parsed, "value");
    double extracted_value = cJSON_GetNumberValue(val_item);
    cJSON *doubled_node = cJSON_CreateNumber(extracted_value * 2.0);
    cJSON_AddItemToObject(root, "doubled", doubled_node);
    cJSON *label_item = cJSON_GetObjectItem(parsed, "label");
    const char *label_text = cJSON_GetStringValue(label_item);
    cJSON_AddStringToObject(root, "label_from_parsed", label_text);

    // step 3: Operate and Validate
    cJSON *retrieved = cJSON_GetObjectItem(root, "doubled");
    double validated_value = cJSON_GetNumberValue(retrieved);
    (void)validated_value; // value used to demonstrate flow; attached data retained in root

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
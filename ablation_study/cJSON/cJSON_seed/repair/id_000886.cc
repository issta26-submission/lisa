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
//<ID> 886
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "label", "example_label");
    cJSON_AddNumberToObject(child, "value", 123.0);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemReferenceToObject(root, "child_ref", child);

    // step 2: Configure
    cJSON *child_ref = cJSON_GetObjectItem(root, "child_ref");
    cJSON *label_item = cJSON_GetObjectItem(child_ref, "label");
    const char *label_str = cJSON_GetStringValue(label_item);
    cJSON *value_item = cJSON_GetObjectItem(child_ref, "value");
    double value_num = cJSON_GetNumberValue(value_item);
    cJSON_AddNumberToObject(root, "copied_value", value_num);

    // step 3: Operate and Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *label_snapshot = cJSON_CreateString(label_str);
    cJSON_AddItemToObject(root, "label_snapshot", label_snapshot);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
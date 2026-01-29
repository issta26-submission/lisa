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
//<ID> 508
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("initial_name");
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddNumberToObject(root, "count", 1.0);

    // step 2: Configure
    cJSON *child = cJSON_CreateObject();
    cJSON_AddNumberToObject(child, "value", 42.0);
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Operate
    cJSON *replacement_name = cJSON_CreateString("final_name");
    cJSON *old_name = cJSON_GetObjectItem(root, "name");
    cJSON_ReplaceItemViaPointer(root, old_name, replacement_name);
    cJSON_AddNumberToObject(root, "added", 3.14);
    cJSON *child_value_item = cJSON_GetObjectItem(child, "value");
    double child_value = cJSON_GetNumberValue(child_value_item);
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_value = cJSON_GetNumberValue(count_item);
    double sum = count_value + child_value;
    cJSON_AddNumberToObject(root, "sum", sum);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate and Cleanup
    cJSON *found_name = cJSON_GetObjectItem(root, "name");
    const char *final_name = cJSON_GetStringValue(found_name);
    (void)final_name;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
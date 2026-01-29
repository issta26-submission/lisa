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
//<ID> 452
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *entry = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("initial_name");
    cJSON_AddItemToObject(entry, "name", name);
    cJSON_AddItemToObject(root, "entry", entry);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("replaced_name");
    cJSON_ReplaceItemViaPointer(root, entry, replacement);
    cJSON *current_entry = cJSON_GetObjectItem(root, "entry");
    const char *entry_value = cJSON_GetStringValue(current_entry);

    // step 3: Operate
    cJSON_bool flag_is_false = cJSON_IsFalse(flag);
    double metric_value = (double)flag_is_false * 3.14159;
    cJSON *metric = cJSON_CreateNumber(metric_value);
    cJSON_AddItemToObject(root, "metric", metric);
    (void)entry_value;

    // step 4: Validate and Cleanup
    char *printed = cJSON_Print(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 181
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *initial_label = (cJSON *)0;
    cJSON *replacement_str = (cJSON *)0;
    cJSON *retrieved_item = (cJSON *)0;
    char *retrieved_text = (char *)0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();

    // step 3: Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddFalseToObject(child, "enabled");
    initial_label = cJSON_CreateString("initial_label");
    cJSON_AddItemToObject(child, "label", initial_label);

    // step 4: Operate
    replacement_str = cJSON_CreateString("replaced_value");
    cJSON_ReplaceItemInObjectCaseSensitive(child, "enabled", replacement_str);

    // step 5: Validate
    retrieved_item = cJSON_GetObjectItemCaseSensitive(child, "enabled");
    retrieved_text = cJSON_GetStringValue(retrieved_item);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
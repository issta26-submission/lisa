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
//<ID> 504
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "count", 3.0);
    cJSON *label = cJSON_CreateString("initial_label");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("replaced_label");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, label, replacement);
    (void)replaced;

    // step 3: Operate
    cJSON_AddNumberToObject(root, "extra", 7.0);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate and Cleanup
    const cJSON *found = cJSON_GetObjectItem(root, "label");
    const char *value = cJSON_GetStringValue(found);
    (void)value;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
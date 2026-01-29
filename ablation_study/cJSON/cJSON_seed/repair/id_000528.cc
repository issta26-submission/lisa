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
//<ID> 528
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *label = cJSON_CreateString("initial_label");
    cJSON_AddItemToObject(root, "label", label);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "active", flag);

    // step 2: Configure
    cJSON *replacement_label = cJSON_CreateString("replaced_label");
    cJSON *label_ptr = cJSON_GetObjectItemCaseSensitive(root, "label");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, label_ptr, replacement_label);
    (void)replaced;

    // step 3: Operate
    cJSON *current_label = cJSON_GetObjectItemCaseSensitive(root, "label");
    char *label_str = cJSON_GetStringValue(current_label);
    cJSON_AddStringToObject(root, "copied_label", label_str);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate and Cleanup
    cJSON *copied = cJSON_GetObjectItemCaseSensitive(root, "copied_label");
    char *copied_str = cJSON_GetStringValue(copied);
    (void)copied_str;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 520
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON *label = cJSON_CreateString("initial_label");
    cJSON_AddItemToObject(root, "label", label);
    cJSON_AddItemToObject(root, "info", info);
    cJSON *inner = cJSON_CreateString("inner_value");
    cJSON_AddItemToObject(info, "inner", inner);

    // step 2: Configure
    const char *label_text = cJSON_GetStringValue(label);
    cJSON *label_copy = cJSON_CreateString(label_text);
    cJSON_AddItemToObject(root, "label_copy", label_copy);
    cJSON *note = cJSON_CreateString("configured");
    cJSON_AddItemToObject(root, "note", note);

    // step 3: Operate
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_bool replaced_ok = cJSON_ReplaceItemViaPointer(root, label, false_item);
    double replaced_flag = (double)replaced_ok;
    cJSON_AddNumberToObject(root, "replace_ok", replaced_flag);
    cJSON *found_label = cJSON_GetObjectItemCaseSensitive(root, "label");
    (void)found_label;
    cJSON *found_copy = cJSON_GetObjectItemCaseSensitive(root, "label_copy");
    const char *copy_text = cJSON_GetStringValue(found_copy);
    cJSON_AddStringToObject(root, "restored_label", copy_text);

    // step 4: Validate and Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
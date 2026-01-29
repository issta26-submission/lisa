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
//<ID> 505
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *label = cJSON_CreateString("old_label");
    cJSON_AddItemToObject(root, "label", label);
    cJSON_AddNumberToObject(root, "count", 3.0);

    // step 2: Configure
    cJSON *sub = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "sub", sub);
    cJSON_AddNumberToObject(sub, "value", 7.5);

    // step 3: Operate
    cJSON *new_label = cJSON_CreateString("new_label");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, label, new_label);
    (void)replaced;
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate and Cleanup
    cJSON *sub_ref = cJSON_GetObjectItem(root, "sub");
    cJSON *val_item = cJSON_GetObjectItem(sub_ref, "value");
    double found = cJSON_GetNumberValue(val_item);
    double scaled = found * 3.0;
    cJSON_AddNumberToObject(root, "scaled", scaled);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
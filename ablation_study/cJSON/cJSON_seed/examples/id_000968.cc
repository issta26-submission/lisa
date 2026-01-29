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
//<ID> 968
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
    cJSON *owner = cJSON_CreateString("ownerA");

    // step 2: Configure
    cJSON_SetValuestring(label, "updated_label");
    cJSON_AddItemToObject(root, "label", label);
    cJSON_AddItemToObject(root, "owner", owner);
    cJSON_AddFalseToObject(root, "enabled");

    // step 3: Operate & Validate
    cJSON *got_label = cJSON_GetObjectItem(root, "label");
    const char *label_str = cJSON_GetStringValue(got_label);
    cJSON *label_copy = cJSON_CreateString(label_str);
    cJSON_AddItemToObject(root, "label_copy", label_copy);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}
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
//<ID> 66
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *name = cJSON_CreateString("tester");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *label = cJSON_CreateString("initial");
    cJSON_AddItemToObject(config, "label", label);

    // step 2: Configure
    char *new_label_ptr = cJSON_SetValuestring(label, "updated");
    (void)new_label_ptr;

    // step 3: Operate and Validate
    cJSON_bool has_cfg = cJSON_HasObjectItem(root, "config");
    const char *name_val = cJSON_GetStringValue(name);
    double validation = (double)has_cfg + (double)name_val[0];
    cJSON_AddNumberToObject(root, "validation", validation);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
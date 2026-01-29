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
//<ID> 766
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
    cJSON_AddNumberToObject(child, "value", 42.0);
    cJSON_AddStringToObject(child, "name", "childnode");
    cJSON *true_item = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_bool added_enabled = cJSON_AddItemToObjectCS(root, "enabled", true_item);
    cJSON *added_bool = cJSON_AddBoolToObject(root, "active", 1);
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_bool added_ref = cJSON_AddItemToObjectCS(root, "child_ref", child_ref);
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    cJSON *dup_child = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObjectCS(root, "child_copy", dup_child);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
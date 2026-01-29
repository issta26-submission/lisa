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
//<ID> 769
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_text[] = " { \"alpha\" : 42, \"beta\" : false } ";
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObjectCS(child, "active", true_item);
    cJSON_AddBoolToObject(child, "enabled", 1);
    cJSON_AddItemToObjectCS(root, "child", child);

    // step 2: Configure
    // use a deep duplicate instead of an object reference to avoid double-free on deletion
    cJSON *child_ref = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObjectCS(root, "childRef", child_ref);
    cJSON_Minify(json_text);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
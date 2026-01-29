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
//<ID> 867
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
    cJSON_AddStringToObject(root, "id", "root_node");
    cJSON_AddNumberToObject(root, "version", 3.14);

    // step 2: Configure
    cJSON *child = cJSON_CreateObject();
    cJSON_AddNumberToObject(child, "count", 42.0);
    cJSON_AddStringToObject(child, "note", "child_entry");
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(child, "optional", null_item);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(child, "active", true_item);
    cJSON_AddItemToObject(config, "child", child);

    // step 3: Operate and Validate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_child = cJSON_GetObjectItem(got_config, "child");
    cJSON *active_item = cJSON_GetObjectItem(got_child, "active");
    double active_flag = (double)cJSON_IsTrue(active_item);
    cJSON_AddNumberToObject(root, "active_flag", active_flag);
    char *snapshot = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
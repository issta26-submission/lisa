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
//<ID> 854
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"config\":{\"threshold\":3.14,\"inner\":{\"value\":42}},\"name\":\"sensor\"}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *config = cJSON_GetObjectItem(root, "config");
    cJSON *inner = cJSON_GetObjectItem(config, "inner");

    // step 2: Configure
    cJSON *new_num = cJSON_CreateNumber(100.0);
    cJSON_AddItemToObject(inner, "added", new_num);
    cJSON *inner_ref = cJSON_CreateObjectReference(inner);
    cJSON_AddItemToObject(root, "inner_ref", inner_ref);

    // step 3: Operate and Validate
    cJSON *inner_ref_item = cJSON_GetObjectItem(root, "inner_ref");
    cJSON *added_item = cJSON_GetObjectItem(inner_ref_item, "added");
    double added_val = cJSON_GetNumberValue(added_item);
    cJSON *copied = cJSON_CreateNumber(added_val + 1.0);
    cJSON_AddItemToObject(root, "copied_plus_one", copied);
    char *snapshot_root = cJSON_PrintUnformatted(root);
    char *snapshot_inner = cJSON_PrintUnformatted(inner);

    // step 4: Cleanup
    cJSON_free(snapshot_root);
    cJSON_free(snapshot_inner);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
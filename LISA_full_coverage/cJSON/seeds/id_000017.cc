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
//<ID> 17
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"num\":123,\"str\":\"hello\"}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);

    // step 2: Configure
    cJSON *root_dup = cJSON_Duplicate(root, 1);
    cJSON *container = cJSON_CreateObject();
    cJSON_AddItemToObject(container, "dup", root_dup);
    cJSON_AddTrueToObject(container, "active");
    cJSON_AddFalseToObject(container, "locked");

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(container);
    cJSON *orig_num_item = cJSON_GetObjectItem(root, "num");
    cJSON *dup_num_item = cJSON_GetObjectItem(root_dup, "num");
    double orig_num = cJSON_GetNumberValue(orig_num_item);
    double dup_num = cJSON_GetNumberValue(dup_num_item);
    cJSON_bool nums_match = cJSON_Compare(orig_num_item, dup_num_item, 1);
    cJSON *active_item = cJSON_GetObjectItem(container, "active");
    cJSON *locked_item = cJSON_GetObjectItem(container, "locked");
    cJSON_bool active_is_true = cJSON_IsTrue(active_item);
    cJSON_bool locked_is_false = cJSON_IsFalse(locked_item);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(container);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 16
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"active\":true,\"count\":3,\"meta\":{\"valid\":false}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON *injected = cJSON_CreateObject();
    cJSON_AddTrueToObject(injected, "forcedTrue");
    cJSON_AddFalseToObject(injected, "forcedFalse");
    cJSON_AddItemToObject(dup, "injected", injected);

    // step 3: Operate and Validate
    cJSON *active_item = cJSON_GetObjectItem(dup, "active");
    double active_numeric = cJSON_GetNumberValue(active_item);
    cJSON_bool equal = cJSON_Compare(root, dup, 0);
    cJSON *equal_node = cJSON_CreateBool(equal);
    cJSON_AddItemToObject(dup, "comparison_equal", equal_node);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 10
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"active\":true,\"items\":[1,2],\"flag\":false}";
    const char *parse_end = 0;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);

    // step 2: Configure
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *items_dup = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(dup_root, "itemsCopy", items_dup);
    cJSON_AddTrueToObject(dup_root, "addedTrue");
    cJSON_AddFalseToObject(dup_root, "addedFalse");

    // step 3: Operate and Validate
    char *serialized = cJSON_PrintUnformatted(dup_root);
    double first_value = cJSON_GetNumberValue(cJSON_GetArrayItem(items_dup, 0));
    cJSON_bool active_is_true = cJSON_IsTrue(cJSON_GetObjectItem(root, "active"));
    cJSON_bool flag_is_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "flag"));
    cJSON_free(serialized);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup_root);

    // API sequence test completed successfully
    return 66;
}
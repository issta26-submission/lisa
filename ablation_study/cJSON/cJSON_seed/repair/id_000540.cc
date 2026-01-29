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
//<ID> 540
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_bool added1 = cJSON_AddItemToArray(items, true_item);
    cJSON *name = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(root, "name", name);

    // step 2: Configure
    cJSON *detached_name = cJSON_DetachItemFromObject(root, "name");
    char *detached_str = cJSON_GetStringValue(detached_name);
    cJSON *name_copy = cJSON_CreateString(detached_str);
    cJSON_AddItemToObject(root, "name_copy", name_copy);

    // step 3: Operate and Validate
    cJSON_bool added2 = cJSON_AddItemToArray(items, cJSON_CreateTrue());
    cJSON *added_count = cJSON_CreateNumber((double)(added1 + added2));
    cJSON_AddItemToObject(root, "added_count", added_count);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(detached_name);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
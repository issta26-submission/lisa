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
//<ID> 1622
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *obj = NULL;
    cJSON *old_flag = NULL;
    cJSON *true_item_created = NULL;
    cJSON *true_added = NULL;
    cJSON *detached = NULL;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    root = cJSON_Parse("{\"obj\": {\"flag\": false}}");
    obj = cJSON_GetObjectItem(root, "obj");
    old_flag = cJSON_GetObjectItem(obj, "flag");

    // step 3: Operate / Validate
    true_added = cJSON_AddTrueToObject(root, "global_flag");
    true_item_created = cJSON_CreateTrue();
    replaced = cJSON_ReplaceItemViaPointer(obj, old_flag, true_item_created);
    detached = cJSON_DetachItemViaPointer(root, true_added);
    cJSON_AddItemToObject(obj, "moved", detached);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1627
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *flag_item = NULL;
    cJSON *move_item = NULL;
    cJSON *replacement = NULL;
    cJSON *detached = NULL;
    cJSON *container = NULL;
    cJSON *added_true = NULL;
    cJSON_bool is_flag_true = 0;

    // step 2: Setup / Configure
    root = cJSON_Parse("{\"flag\":false,\"moveMe\":{\"a\":1}}");
    container = cJSON_CreateObject();

    // step 3: Operate / Validate
    flag_item = cJSON_GetObjectItem(root, "flag");
    replacement = cJSON_CreateTrue();
    cJSON_ReplaceItemViaPointer(root, flag_item, replacement);
    move_item = cJSON_GetObjectItem(root, "moveMe");
    detached = cJSON_DetachItemViaPointer(root, move_item);
    added_true = cJSON_AddTrueToObject(detached, "innerFlag");
    cJSON_AddItemToObject(container, "newParent", detached);
    is_flag_true = cJSON_IsTrue(cJSON_GetObjectItem(root, "flag"));

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(container);

    // API sequence test completed successfully
    (void)flag_item;
    (void)replacement;
    (void)added_true;
    (void)is_flag_true;
    return 66;
}
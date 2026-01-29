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
//<ID> 81
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *old_item = cJSON_CreateObject();
    cJSON *replacement = cJSON_CreateFalse();
    cJSON *item_ptr = (cJSON *)0;
    cJSON *after = (cJSON *)0;
    cJSON_bool was_false = 0;

    // step 2: Setup
    cJSON_AddItemToObject(root, "flag", old_item);

    // step 3: Operate
    item_ptr = cJSON_GetObjectItem(root, "flag");

    // step 4: Replace
    cJSON_ReplaceItemViaPointer(root, item_ptr, replacement);

    // step 5: Validate
    after = cJSON_GetObjectItem(root, "flag");
    was_false = cJSON_IsFalse(after);
    cJSON_AddNumberToObject(root, "flag_was_false", (double)was_false);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
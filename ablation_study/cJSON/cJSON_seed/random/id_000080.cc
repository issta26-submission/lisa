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
//<ID> 80
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *old_item = (cJSON *)0;
    cJSON *replacement = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON_bool replaced = 0;
    cJSON_bool is_false = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    old_item = cJSON_CreateObject();

    // step 3: Configure - attach the old item to the root under the key "target"
    cJSON_AddItemToObject(root, "target", old_item);

    // step 4: Operate - create a false item and replace the previously attached item via pointer
    replacement = cJSON_CreateFalse();
    replaced = cJSON_ReplaceItemViaPointer(root, old_item, replacement);

    // step 5: Validate - retrieve the item by name and check it's false
    retrieved = cJSON_GetObjectItem(root, "target");
    is_false = cJSON_IsFalse(retrieved);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)replaced;
    (void)is_false;
    return 66;
}
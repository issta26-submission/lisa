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
//<ID> 85
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *old_item = cJSON_CreateString("old_value");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *retrieved = (cJSON *)0;
    cJSON_bool replaced = 0;
    cJSON_bool is_false = 0;

    // step 2: Setup - attach an initial item to the root object
    cJSON_AddItemToObject(root, "node", old_item);

    // step 3: Operate - replace the attached item via pointer with a false item
    replaced = cJSON_ReplaceItemViaPointer(root, old_item, false_item);

    // step 4: Validate - retrieve the node and record its false-ness back into the root
    retrieved = cJSON_GetObjectItem(root, "node");
    is_false = cJSON_IsFalse(retrieved);
    cJSON_AddItemToObject(root, "node_is_false", cJSON_CreateBool(is_false));

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
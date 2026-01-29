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
//<ID> 84
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
    cJSON *new_item = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON_bool replaced = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    old_item = cJSON_CreateString("original_value");
    cJSON_AddItemToObject(root, "key", old_item);

    // step 3: Configure - create replacement and replace via pointer
    new_item = cJSON_CreateFalse();
    replaced = cJSON_ReplaceItemViaPointer(root, old_item, new_item);

    // step 4: Operate - retrieve the item and record validation info into the root
    retrieved = cJSON_GetObjectItem(root, "key");
    cJSON_AddNumberToObject(root, "replaced_flag", (double)replaced);
    cJSON_AddNumberToObject(root, "is_false_flag", (double)cJSON_IsFalse(retrieved));

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
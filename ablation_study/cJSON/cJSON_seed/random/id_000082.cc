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
//<ID> 82
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *initial = cJSON_CreateString("active");
    cJSON *replacement = cJSON_CreateFalse();
    cJSON *retrieved = (cJSON *)0;
    cJSON_bool replaced = 0;

    // step 2: Setup - attach an initial item to the root object
    cJSON_AddItemToObject(root, "mode", initial);

    // step 3: Operate - replace the attached item via pointer with a false boolean
    replaced = cJSON_ReplaceItemViaPointer(root, initial, replacement);

    // step 4: Validate - retrieve the item and record whether it is false
    retrieved = cJSON_GetObjectItem(root, "mode");
    cJSON_AddBoolToObject(root, "was_false", cJSON_IsFalse(retrieved));

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
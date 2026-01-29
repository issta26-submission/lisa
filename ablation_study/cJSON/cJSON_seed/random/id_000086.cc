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
//<ID> 86
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations & initialization
    cJSON *root = cJSON_CreateObject();
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON *replacement_str = cJSON_CreateString("replaced_flag");
    cJSON *retrieved = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    const char *retrieved_text = (const char *)0;

    // step 2: Setup - attach initial items to root
    cJSON_AddItemToObject(root, "flag", flag_false);
    cJSON_AddNumberToObject(root, "value", 7.0);

    // step 3: Replace the previously attached item via pointer
    cJSON_ReplaceItemViaPointer(root, flag_false, replacement_str);

    // step 4: Operate - read back the replaced item and reuse its data
    retrieved = cJSON_GetObjectItem(root, "flag");
    retrieved_text = cJSON_GetStringValue(retrieved);
    cJSON_AddItemToObject(root, "first_value", cJSON_CreateString(retrieved_text));

    // step 5: Validate-like operation - detach a child and delete it explicitly
    detached = cJSON_DetachItemFromObject(root, "value");
    cJSON_Delete(detached);

    // step 6: Cleanup - delete the root (which deletes remaining children)
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
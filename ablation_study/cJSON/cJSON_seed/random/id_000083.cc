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
//<ID> 83
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *item_false = (cJSON *)0;
    cJSON *replacement = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool replaced = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    item_false = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToObject(root, "flag", item_false);

    // step 4: Operate
    replacement = cJSON_CreateString("was_false");
    replaced = cJSON_ReplaceItemViaPointer(root, item_false, replacement);

    // step 5: Validate
    retrieved = cJSON_GetObjectItem(root, "flag");
    printed = cJSON_PrintUnformatted(root);
    cJSON_AddNumberToObject(root, "replaced", (double)replaced);

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
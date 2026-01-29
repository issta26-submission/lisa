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
//<ID> 1108
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *str_item = NULL;
    cJSON *num_item = NULL;
    cJSON *detached = NULL;
    cJSON *bool_item = NULL;
    char *printed = NULL;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("to_be_replaced");
    cJSON_AddItemToObject(root, "replace_me", str_item);
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "temp", num_item);

    // step 3: Operate / Validate
    detached = cJSON_DetachItemFromObject(root, "temp");
    bool_item = cJSON_CreateBool(0);
    replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "replace_me", bool_item);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
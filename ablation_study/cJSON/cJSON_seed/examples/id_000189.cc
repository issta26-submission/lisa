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
//<ID> 189
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", array);
    cJSON *elem0 = cJSON_CreateString("first");
    cJSON_AddItemToArray(array, elem0);
    cJSON *elem1 = cJSON_CreateString("second");
    cJSON_AddItemToArray(array, elem1);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateStringReference("replaced");
    cJSON_bool replaced = cJSON_ReplaceItemInArray(array, 1, replacement);

    // step 3: Operate & Validate
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, "arr");
    cJSON_AddItemToObject(root, "new_array", detached);
    cJSON *ref = cJSON_CreateStringReference("a_ref_string");
    cJSON_AddItemToObject(root, "ref", ref);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}
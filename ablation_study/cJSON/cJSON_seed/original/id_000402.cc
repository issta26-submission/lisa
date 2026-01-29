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
//<ID> 402
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "status", "initial");
    cJSON *numbers = cJSON_AddArrayToObject(root, "numbers");
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(3.0));
    cJSON_AddStringToObject(root, "mode", "legacy");

    // step 2: Configure
    cJSON *new_mode = cJSON_CreateString("updated");
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "mode", new_mode);
    cJSON *second_before = cJSON_GetArrayItem(numbers, 1);
    cJSON_bool second_invalid_before = cJSON_IsInvalid(second_before);
    cJSON_AddBoolToObject(root, "second_invalid_before", second_invalid_before);

    // step 3: Operate
    cJSON_DeleteItemFromArray(numbers, 1);
    cJSON *second_after = cJSON_GetArrayItem(numbers, 1);
    cJSON_bool second_invalid_after = cJSON_IsInvalid(second_after);
    cJSON_AddBoolToObject(root, "second_invalid_after", second_invalid_after);
    (void)replaced; // preserve use of replaced result in a no-op way

    // step 4: Validate and Cleanup
    char *snapshot = cJSON_Print(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
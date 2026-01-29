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
//<ID> 38
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
    cJSON_AddItemToObject(root, "list", array);

    // step 2: Configure
    cJSON *item0 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(array, item0);
    cJSON *item1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(array, item1);
    cJSON *item2 = cJSON_CreateTrue();
    cJSON_AddItemToArray(array, item2);
    cJSON_AddItemToObject(root, "status", cJSON_CreateString("ok"));

    // step 3: Operate & Validate
    cJSON *replacement = cJSON_CreateString("BETA_REPLACED");
    cJSON_bool replaced = cJSON_ReplaceItemInArray(array, 1, replacement);
    cJSON *third = cJSON_GetArrayItem(array, 2);
    cJSON_bool is_true = cJSON_IsTrue(third);
    cJSON *first = cJSON_GetArrayItem(array, 0);
    char *first_val = cJSON_GetStringValue(first);
    cJSON_AddItemToObject(root, "copied", cJSON_CreateString(first_val));
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    (void)replaced;
    (void)is_true;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 36
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
    cJSON_AddItemToObject(root, "items", array);

    // step 2: Configure
    cJSON *str0 = cJSON_CreateString("alpha");
    cJSON *str1 = cJSON_CreateString("beta");
    cJSON *bool_true = cJSON_CreateTrue();
    cJSON_AddItemToArray(array, str0);
    cJSON_AddItemToArray(array, bool_true);
    cJSON_AddItemToArray(array, str1);

    // step 3: Operate & Validate
    cJSON_bool was_true = cJSON_IsTrue(bool_true);
    cJSON *first_item = cJSON_GetArrayItem(array, 0);
    char *first_value = cJSON_GetStringValue(first_item);
    cJSON *replacement = cJSON_CreateString("replaced_true");
    cJSON_bool replaced = cJSON_ReplaceItemInArray(array, 1, replacement);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    (void)was_true;
    (void)first_value;
    (void)replaced;

    // API sequence test completed successfully
    return 66;
}
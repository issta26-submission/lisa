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
//<ID> 35
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
    cJSON *item0 = cJSON_CreateString("alpha");
    cJSON *item1 = cJSON_CreateTrue();
    cJSON *item2 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(array, item0);
    cJSON_AddItemToArray(array, item1);
    cJSON_AddItemToArray(array, item2);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateFalse();
    cJSON_ReplaceItemInArray(array, 1, replacement);
    cJSON_bool was_true_after_replace = cJSON_IsTrue(cJSON_GetArrayItem(array, 1));

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(array, 0);
    char *first_value = cJSON_GetStringValue(first);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    (void)first_value;
    (void)was_true_after_replace;
    // API sequence test completed successfully
    return 66;
}
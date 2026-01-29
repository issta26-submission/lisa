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
//<ID> 33
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
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", flag);
    cJSON *s1 = cJSON_CreateString("first");
    cJSON *s2 = cJSON_CreateString("second");
    cJSON_AddItemToArray(array, s1);
    cJSON_AddItemToArray(array, s2);
    cJSON_AddItemToArray(array, cJSON_CreateString("third"));

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("SECOND_REPLACED");
    cJSON_AddItemToObject(root, "note", cJSON_CreateString("sequence_test"));

    // step 3: Operate & Validate
    cJSON_bool replaced = cJSON_ReplaceItemInArray(array, 1, replacement);
    cJSON_bool enabled_true = cJSON_IsTrue(flag);
    cJSON *fetched = cJSON_GetArrayItem(array, 1);
    char *fetched_str = cJSON_GetStringValue(fetched);
    char *out = cJSON_PrintUnformatted(root);
    (void)replaced;
    (void)enabled_true;
    (void)fetched_str;
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
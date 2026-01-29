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
//<ID> 391
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
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *s2 = cJSON_CreateString("two");
    cJSON *t = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "arr", array);

    // step 2: Configure and Operate
    cJSON_AddItemToArray(array, s1);
    cJSON_InsertItemInArray(array, 0, t);
    cJSON_AddItemToArray(array, s2);
    cJSON *detached = cJSON_DetachItemFromArray(array, 2);
    char *updated = cJSON_SetValuestring(detached, "two-modified");
    (void)updated;
    cJSON_InsertItemInArray(array, 1, detached);

    // step 3: Validate
    cJSON *item0 = cJSON_GetArrayItem(array, 0);
    cJSON *item1 = cJSON_GetArrayItem(array, 1);
    cJSON *item2 = cJSON_GetArrayItem(array, 2);
    cJSON_bool item0_is_true = cJSON_IsTrue(item0);
    const char *item1_str = cJSON_GetStringValue(item1);
    const char *item2_str = cJSON_GetStringValue(item2);
    char *out = cJSON_PrintUnformatted(root);
    (void)item0_is_true;
    (void)item1_str;
    (void)item2_str;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
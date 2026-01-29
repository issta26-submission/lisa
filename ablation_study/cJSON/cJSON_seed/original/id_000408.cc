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
//<ID> 408
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(items, n1);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(items, n2);
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddItemToObject(obj, "name", cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(items, obj);
    cJSON *to_replace = cJSON_CreateNumber(1.5);
    cJSON_AddItemToObject(root, "replace_me", to_replace);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateObject();
    cJSON_AddItemToObject(replacement, "replaced", cJSON_CreateString("yes"));
    cJSON_ReplaceItemInObjectCaseSensitive(root, "replace_me", replacement);

    // step 3: Operate
    int size_before = cJSON_GetArraySize(items);
    cJSON_DeleteItemFromArray(items, 0);
    cJSON *first_after_delete = cJSON_GetArrayItem(items, 0);
    cJSON_bool was_invalid = cJSON_IsInvalid(first_after_delete);
    double extracted = cJSON_GetNumberValue(first_after_delete);
    cJSON *computed = cJSON_CreateNumber(extracted + (was_invalid ? 0.0 : 5.0));
    cJSON_AddItemToArray(items, computed);

    // step 4: Validate and Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
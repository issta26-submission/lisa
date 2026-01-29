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
//<ID> 14
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
    cJSON *countItem = cJSON_AddNumberToObject(root, "count", 1.0);

    // step 2: Configure
    cJSON *first = cJSON_CreateString("first");
    cJSON_AddItemToArray(array, first);
    cJSON *replacement = cJSON_CreateString("replaced_first");
    cJSON *cmp_a = cJSON_CreateString("alpha");
    cJSON *cmp_b = cJSON_CreateString("alpha");
    cJSON *nullItem = cJSON_CreateNull();

    // step 3: Operate & Validate
    cJSON_bool replaced = cJSON_ReplaceItemInArray(array, 0, replacement);
    (void)replaced;
    cJSON_bool equal = cJSON_Compare(cmp_a, cmp_b, 1);
    (void)equal;
    cJSON_bool was_null = cJSON_IsNull(nullItem);
    (void)was_null;
    (void)countItem;

    // step 4: Cleanup
    cJSON_Delete(cmp_a);
    cJSON_Delete(cmp_b);
    cJSON_Delete(nullItem);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
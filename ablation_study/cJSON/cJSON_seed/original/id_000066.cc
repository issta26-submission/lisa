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
//<ID> 66
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *node = cJSON_CreateObject();
    cJSON_AddNumberToObject(node, "id", 321.0);
    cJSON_AddStringToObject(node, "label", "nodeA");
    cJSON_AddItemToArray(items, node);

    // step 2: Configure
    cJSON *node_ref = cJSON_CreateObjectReference(node);
    cJSON_AddItemReferenceToArray(items, node_ref);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON *second = cJSON_GetArrayItem(items, 1);
    cJSON_bool first_is_obj = cJSON_IsObject(first);
    cJSON_bool second_is_obj = cJSON_IsObject(second);
    cJSON_bool items_equal = cJSON_Compare(first, second, 1);
    int summary = (int)first_is_obj + (int)second_is_obj + (int)items_equal;
    (void)summary;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
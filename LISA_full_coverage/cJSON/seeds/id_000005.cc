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
//<ID> 5
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *str_ref_in_child = cJSON_CreateStringReference("hello from child");
    cJSON *num_in_child = cJSON_CreateNumber(123.0);
    cJSON *arr_str_ref = cJSON_CreateStringReference("array element");

    // step 2: Configure - build a small structure and references
    cJSON_AddItemToObject(child, "greeting", str_ref_in_child);
    cJSON_AddItemToObject(child, "count", num_in_child);
    cJSON_AddItemToObject(root, "child", child);
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);
    cJSON_AddItemToArray(arr, arr_str_ref);
    cJSON_AddItemToObject(root, "arr", arr);

    // step 3: Operate / Validate - move an array item into root and remove original object
    cJSON *detached = cJSON_DetachItemFromArray(arr, 0);
    cJSON_AddItemToObject(root, "moved_element", detached);
    cJSON_DeleteItemFromObject(root, "child");

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
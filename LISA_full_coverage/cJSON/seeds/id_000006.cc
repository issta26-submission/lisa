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
//<ID> 6
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *obj = cJSON_CreateObject();
    cJSON *str_ref = cJSON_CreateStringReference("hello");
    cJSON *num_child = cJSON_AddNumberToObject(obj, "value", 123.0);

    // step 2: Setup - assemble structure
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddItemToObject(root, "obj", obj);
    cJSON_AddItemToArray(arr, str_ref);
    cJSON *obj_ref = cJSON_CreateObjectReference(num_child);
    cJSON_AddItemToObject(root, "ref", obj_ref);

    // step 3: Operate / Validate
    cJSON *detached = cJSON_DetachItemFromArray(arr, 0);
    cJSON_DeleteItemFromObject(root, "ref");

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
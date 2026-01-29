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
//<ID> 1
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *boolItem = cJSON_CreateBool(1);
    cJSON *strItem = cJSON_CreateString("initial");
    cJSON *numItem = cJSON_CreateNumber(3.14);

    // step 2
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToArray(arr, boolItem);
    cJSON_AddItemToArray(arr, strItem);
    cJSON_AddItemToArray(arr, numItem);
    cJSON_SetValuestring(strItem, "updated");

    // step 3
    cJSON_bool is_bool = cJSON_IsBool(boolItem);
    cJSON *objBool = cJSON_AddBoolToObject(root, "active", 0);
    cJSON_bool is_obj_bool = cJSON_IsBool(objBool);
    (void)is_bool;
    (void)is_obj_bool;
    cJSON_DeleteItemFromArray(arr, 1);

    // step 4
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
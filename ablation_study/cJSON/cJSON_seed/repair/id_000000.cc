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
//<ID> 0
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *bool_item = cJSON_CreateBool(1);
    cJSON *str_item = cJSON_CreateString("initial");
    cJSON_bool is_bool_in_array;
    cJSON_bool is_bool_in_object;
    char *updated_string_ptr;

    // step 2: Setup - attach array to root and populate
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToArray(arr, bool_item);

    // step 3: Operate / Validate - add a bool to object, modify string, and validate types
    cJSON *obj_bool = cJSON_AddBoolToObject(root, "flag", 0);
    updated_string_ptr = cJSON_SetValuestring(str_item, "updated");
    cJSON_AddItemToArray(arr, str_item);
    is_bool_in_array = cJSON_IsBool(bool_item);
    is_bool_in_object = cJSON_IsBool(obj_bool);

    // step 4: Cleanup
    cJSON_DeleteItemFromArray(arr, 0);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)is_bool_in_array;
    (void)is_bool_in_object;
    (void)updated_string_ptr;
    return 66;
}
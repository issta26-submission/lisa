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
//<ID> 37
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root_obj = cJSON_CreateObject();
    cJSON *root_array = cJSON_CreateArray();
    cJSON *bool_item = cJSON_CreateBool(0);
    cJSON *detached_item = (cJSON *)0;
    int arr_size_after_detach = 0;
    cJSON_bool add_result_array = 0;
    cJSON_bool add_result_obj_arr = 0;
    cJSON_bool add_result_moved = 0;
    cJSON_bool has_moved = 0;

    // step 2: Setup
    add_result_array = cJSON_AddItemToArray(root_array, bool_item);
    add_result_obj_arr = cJSON_AddItemToObjectCS(root_obj, "arr", root_array);
    cJSON *true_child = cJSON_AddTrueToObject(root_obj, "initial_true");

    // step 3: Operate
    detached_item = cJSON_DetachItemFromArray(root_array, 0);
    add_result_moved = cJSON_AddItemToObjectCS(root_obj, "moved", detached_item);

    // step 4: Validate
    arr_size_after_detach = cJSON_GetArraySize(root_array);
    has_moved = cJSON_HasObjectItem(root_obj, "moved");
    (void)add_result_array;
    (void)add_result_obj_arr;
    (void)true_child;
    (void)add_result_moved;
    (void)arr_size_after_detach;
    (void)has_moved;

    // step 5: Cleanup
    cJSON_Delete(root_obj);

    // API sequence test completed successfully
    return 66;
}
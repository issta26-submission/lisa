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
//<ID> 1641
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *child_str = (cJSON *)0;
    cJSON *child_num = (cJSON *)0;
    cJSON *detached_via_ptr = (cJSON *)0;
    cJSON *detached_from_array = (cJSON *)0;
    char json_buf[] = " { \"alpha\" : 123 , \"beta\" : [ 4 , 5 ] } ";
    cJSON_bool added_to_object = 0;
    cJSON_bool added_to_array = 0;
    cJSON_bool added_array_to_object = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    child_str = cJSON_CreateString("moved");
    child_num = cJSON_CreateNumber(42.0);

    // step 3: Configure
    added_to_object = cJSON_AddItemToObject(root, "first", child_str);
    added_array_to_object = cJSON_AddItemToObject(root, "values", arr);
    added_to_array = cJSON_AddItemToArray(arr, child_num);

    // step 4: Operate
    detached_via_ptr = cJSON_DetachItemViaPointer(root, child_str);
    cJSON_AddItemToArray(arr, detached_via_ptr);
    detached_from_array = cJSON_DetachItemFromArray(arr, 0);
    cJSON_Minify(json_buf);

    // step 5: Validate
    validation_score = (int)added_to_object
        + (int)added_array_to_object
        + (int)added_to_array
        + (int)(detached_via_ptr != (cJSON *)0)
        + (int)(detached_from_array != (cJSON *)0)
        + (int)(cJSON_GetArraySize(arr) == 1);

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached_from_array);
    (void)validation_score;
    (void)json_buf;
    (void)detached_via_ptr;
    (void)child_num;
    (void)child_str;
    (void)added_to_object;
    (void)added_to_array;
    (void)added_array_to_object;
    // API sequence test completed successfully
    return 66;
}
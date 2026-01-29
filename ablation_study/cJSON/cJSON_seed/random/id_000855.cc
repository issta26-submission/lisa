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
//<ID> 855
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
    cJSON *detached = (cJSON *)0;
    cJSON *obj = (cJSON *)0;
    cJSON_bool added_detached = 0;
    cJSON_bool added_obj_to_root = 0;
    cJSON_bool added_number = 0;
    cJSON_bool is_num_flag = 0;
    int validation_score = 0;

    // step 2: Initialize (parse JSON)
    root = cJSON_Parse("{\"arr\":[10,20,30],\"meta\":{\"name\":\"example\"}}");
    arr = cJSON_GetObjectItem(root, "arr");

    // step 3: Configure (detach middle element and wrap it into a new object)
    detached = cJSON_DetachItemFromArray(arr, 1);
    obj = cJSON_CreateObject();
    added_detached = cJSON_AddItemToObject(obj, "value", detached);
    added_obj_to_root = cJSON_AddItemToObject(root, "detached_wrapper", obj);

    // step 4: Operate (check type and add an additional number)
    is_num_flag = cJSON_IsNumber(detached);
    added_number = cJSON_AddItemToObject(root, "extra", cJSON_CreateNumber(99.0));

    // step 5: Validate (collect simple indicators)
    validation_score = (root != (cJSON *)0) + (arr != (cJSON *)0) + (detached != (cJSON *)0) + (obj != (cJSON *)0) + (int)is_num_flag + (int)added_detached + (int)added_obj_to_root + (int)added_number;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
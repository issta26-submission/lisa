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
//<ID> 852
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
    cJSON *detached_item = (cJSON *)0;
    cJSON *target_obj = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    const char *json = "{\"array\":[10,20,30],\"target\":{},\"num\":123}";
    cJSON_bool added = 0;
    cJSON_bool is_num_flag = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_Parse(json);

    // step 3: Configure
    arr = cJSON_GetObjectItem(root, "array");
    target_obj = cJSON_GetObjectItem(root, "target");
    num_item = cJSON_GetObjectItem(root, "num");

    // step 4: Operate
    detached_item = cJSON_DetachItemFromArray(arr, 1);
    added = cJSON_AddItemToObject(target_obj, "moved", detached_item);
    is_num_flag = cJSON_IsNumber(num_item);

    // step 5: Validate
    validation_score = (root != (cJSON *)0) + (arr != (cJSON *)0) + (detached_item != (cJSON *)0) + (target_obj != (cJSON *)0) + (num_item != (cJSON *)0) + (int)added + (int)is_num_flag;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
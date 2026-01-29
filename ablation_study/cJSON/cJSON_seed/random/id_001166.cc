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
//<ID> 1166
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON *fetched_child = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_elem0 = (cJSON *)0;
    cJSON_bool is_bool_flag = 0;
    int validation_score = 0;

    // step 2: Initialize - create root and child, attach child to root
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Configure - create an array under child and add a boolean element
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(child, "values", arr);
    bool_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, bool_item);

    // step 4: Operate - retrieve nested object and array element, test type, add result back to root
    fetched_child = cJSON_GetObjectItem(root, "child");
    fetched_arr = cJSON_GetObjectItem(fetched_child, "values");
    fetched_elem0 = cJSON_GetArrayItem(fetched_arr, 0);
    is_bool_flag = cJSON_IsBool(fetched_elem0);
    cJSON_AddItemToObject(root, "flag_checked", cJSON_CreateBool(is_bool_flag));

    // step 5: Validate - compute a simple validation score using retrieved data and presence checks
    validation_score = (int)(fetched_child != (cJSON *)0) + (int)(fetched_arr != (cJSON *)0) + (int)(is_bool_flag) + (int)(cJSON_GetObjectItem(root, "flag_checked") != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
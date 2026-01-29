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
//<ID> 996
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "items";
    const char *num_key = "pi";
    const char *bool_key = "active";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num_item1 = (cJSON *)0;
    cJSON *num_item2 = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON_bool add_ok_array = (cJSON_bool)0;
    cJSON_bool add_ok_obj = (cJSON_bool)0;
    cJSON_bool add_ok_num2 = (cJSON_bool)0;
    cJSON_bool is_raw = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item1 = cJSON_CreateNumber(3.14);
    num_item2 = cJSON_CreateNumber(2.718);

    // step 3: Configure
    add_ok_array = cJSON_AddItemToArray(arr, num_item1);
    add_ok_obj = cJSON_AddItemToObject(root, arr_key, arr);
    add_ok_num2 = cJSON_AddItemToObject(root, num_key, num_item2);
    bool_item = cJSON_AddBoolToObject(root, bool_key, (cJSON_bool)1);

    // step 4: Operate
    is_raw = cJSON_IsRaw(num_item2);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0) + (int)add_ok_array + (int)add_ok_obj + (int)add_ok_num2 + (int)(bool_item != (cJSON *)0) + (int)(is_raw == (cJSON_bool)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
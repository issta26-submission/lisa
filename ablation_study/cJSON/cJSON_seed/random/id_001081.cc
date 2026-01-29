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
//<ID> 1081
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
    cJSON *dup_arr = (cJSON *)0;
    cJSON *name_str = (cJSON *)0;
    cJSON *null_item = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_name = (cJSON *)0;
    cJSON *fetched_meta = (cJSON *)0;
    char *printed = (char *)0;
    float nums[3];
    cJSON_bool add_ok_arr = (cJSON_bool)0;
    cJSON_bool add_ok_name = (cJSON_bool)0;
    cJSON_bool add_ok_meta = (cJSON_bool)0;
    cJSON_bool is_null = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - prepare data and create JSON items
    nums[0] = 1.5f;
    nums[1] = 2.5f;
    nums[2] = 3.5f;
    root = cJSON_CreateObject();
    arr = cJSON_CreateFloatArray(nums, 3);
    name_str = cJSON_CreateString("sensor");
    null_item = cJSON_CreateNull();
    add_ok_arr = cJSON_AddItemToObject(root, "values", arr);
    add_ok_name = cJSON_AddItemToObject(root, "name", name_str);
    add_ok_meta = cJSON_AddItemToObject(root, "meta", null_item);

    // step 3: Operate - fetch items, duplicate an array, and check null status
    fetched_arr = cJSON_GetObjectItem(root, "values");
    fetched_name = cJSON_GetObjectItem(root, "name");
    fetched_meta = cJSON_GetObjectItem(root, "meta");
    dup_arr = cJSON_Duplicate(fetched_arr, (cJSON_bool)1);
    is_null = cJSON_IsNull(fetched_meta);

    // step 4: Validate - serialize and derive a small validation score
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(name_str != (cJSON *)0) + (int)(null_item != (cJSON *)0) + (int)(add_ok_arr) + (int)(add_ok_name) + (int)(add_ok_meta) + (int)(dup_arr != (cJSON *)0) + (int)(is_null);
    (void)validation_score;

    // step 5: Cleanup - free all allocated cJSON structures
    cJSON_Delete(root);
    cJSON_Delete(dup_arr);

    // API sequence test completed successfully
    return 66;
}
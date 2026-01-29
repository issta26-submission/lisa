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
//<ID> 517
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *int_arr = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *got_arr = (cJSON *)0;
    cJSON *got_name = (cJSON *)0;
    const char *name_str = (const char *)0;
    int nums[4] = {1, 2, 3, 4};
    int arr_size = 0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    int_arr = cJSON_CreateIntArray(nums, 4);
    name_item = cJSON_CreateString("example");

    // step 3: Configure
    add_ok1 = cJSON_AddItemToObject(root, "numbers", int_arr);
    add_ok2 = cJSON_AddItemToObject(root, "name", name_item);

    // step 4: Operate
    got_arr = cJSON_GetObjectItem(root, "numbers");
    got_name = cJSON_GetObjectItem(root, "name");
    arr_size = cJSON_GetArraySize(got_arr);
    name_str = cJSON_GetStringValue(got_name);

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)arr_size;
    (void)name_str;

    // step 6: Cleanup
    cJSON_Delete(root);

    return 66; // API sequence test completed successfully
}
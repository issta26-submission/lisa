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
//<ID> 534
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
    cJSON *num_array = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *title_item = (cJSON *)0;
    char *title_val = (char *)0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    cJSON_bool is_raw = 0;
    double nums[3];

    // step 2: Initialize
    nums[0] = 1.1;
    nums[1] = 2.2;
    nums[2] = 3.3;
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_array = cJSON_CreateDoubleArray(nums, 3);
    raw_item = cJSON_CreateRaw("raw_payload");
    title_item = cJSON_CreateString("example_dataset");

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, num_array);
    add_ok2 = cJSON_AddItemToArray(arr, raw_item);
    add_ok3 = cJSON_AddItemToObject(root, "entries", arr);
    cJSON_AddItemToObject(root, "title", title_item);

    // step 4: Operate
    is_raw = cJSON_IsRaw(raw_item);
    title_val = cJSON_GetStringValue(title_item);

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)is_raw;
    (void)title_val;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
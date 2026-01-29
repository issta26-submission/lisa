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
//<ID> 515
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *ints = (cJSON *)0;
    cJSON *label = (cJSON *)0;
    cJSON *got_ints = (cJSON *)0;
    cJSON *got_label = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    int nums[4] = {10, 20, 30, 40};
    int arr_size = 0;
    double first_value = 0.0;
    cJSON_bool add_ok = 0;
    char scratch[128];
    memset(scratch, 0, sizeof(scratch));

    // step 2: Initialize
    root = cJSON_CreateObject();
    ints = cJSON_CreateIntArray(nums, 4);
    label = cJSON_CreateString("numbers-array");

    // step 3: Configure
    add_ok = cJSON_AddItemToObject(root, "ints", ints);
    add_ok = cJSON_AddItemToObject(root, "name", label);

    // step 4: Operate
    got_ints = cJSON_GetObjectItem(root, "ints");
    got_label = cJSON_GetObjectItem(root, "name");
    arr_size = cJSON_GetArraySize(got_ints);
    first_item = cJSON_GetArrayItem(got_ints, 0);
    first_value = cJSON_GetNumberValue(first_item);

    // step 5: Validate
    (void)arr_size;
    (void)first_value;
    (void)add_ok;
    (void)got_label;
    (void)scratch;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
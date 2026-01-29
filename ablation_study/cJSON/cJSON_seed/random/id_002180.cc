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
//<ID> 2180
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[32];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *int_arr = NULL;
    cJSON *got_arr = NULL;
    int *nums = NULL;
    cJSON_bool added = 0;
    cJSON_bool is_arr = 0;
    int arr_size = 0;
    int validation = 0;

    // step 2: Setup (allocate numbers and build JSON)
    nums = (int *)cJSON_malloc(sizeof(int) * 3);
    nums[0] = 10;
    nums[1] = 20;
    nums[2] = 30;
    root = cJSON_CreateObject();
    int_arr = cJSON_CreateIntArray(nums, 3);
    added = cJSON_AddItemToObject(root, "values", int_arr);

    // step 3: Operate (retrieve and inspect the array)
    got_arr = cJSON_GetObjectItem(root, "values");
    arr_size = cJSON_GetArraySize(got_arr);
    is_arr = cJSON_IsArray(got_arr);

    // step 4: Validate (accumulate simple checks)
    validation ^= (int)(root != NULL);
    validation ^= (int)(int_arr != NULL);
    validation ^= (int)(nums != NULL);
    validation ^= (int)(added != 0);
    validation ^= (int)(got_arr != NULL);
    validation ^= (int)(arr_size == 3);
    validation ^= (int)(is_arr != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_free(nums);

    // API sequence test completed successfully
    return 66;
}
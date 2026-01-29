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
//<ID> 250
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
    cJSON *answer_num = (cJSON *)0;
    cJSON *count_num = (cJSON *)0;
    cJSON *greet_str = (cJSON *)0;
    char *printed = (char *)0;
    int nums[4] = {10, 20, 30, 40};
    int arr_size = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    int_arr = cJSON_CreateIntArray(nums, 4);
    cJSON_AddItemToObject(root, "ints", int_arr);

    // step 3: Configure
    answer_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", answer_num);
    greet_str = cJSON_AddStringToObject(root, "greeting", "hello");

    // step 4: Operate
    arr_size = cJSON_GetArraySize(int_arr);
    count_num = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "count", count_num);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
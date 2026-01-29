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
//<ID> 258
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
    cJSON *count_num = (cJSON *)0;
    cJSON *avg_num = (cJSON *)0;
    cJSON *pi_num = (cJSON *)0;
    cJSON *str_node = (cJSON *)0;
    cJSON_bool add_ok_arr = (cJSON_bool)0;
    cJSON_bool add_ok_count = (cJSON_bool)0;
    cJSON_bool add_ok_avg = (cJSON_bool)0;
    cJSON_bool add_ok_pi = (cJSON_bool)0;
    int nums[4];
    int size = 0;
    double average = 0.0;
    char *printed = (char *)0;

    // step 2: Initialize
    nums[0] = 1;
    nums[1] = 2;
    nums[2] = 3;
    nums[3] = 4;
    root = cJSON_CreateObject();
    int_arr = cJSON_CreateIntArray(nums, 4);

    // step 3: Configure
    add_ok_arr = cJSON_AddItemToObject(root, "ints", int_arr);
    str_node = cJSON_AddStringToObject(root, "description", "Sample integer array");
    pi_num = cJSON_CreateNumber(3.14159);
    add_ok_pi = cJSON_AddItemToObject(root, "pi", pi_num);

    // step 4: Operate
    size = cJSON_GetArraySize(int_arr);
    average = (1.0 + 2.0 + 3.0 + 4.0) / (double)size;
    avg_num = cJSON_CreateNumber(average);
    count_num = cJSON_CreateNumber((double)size);
    add_ok_avg = cJSON_AddItemToObject(root, "average", avg_num);
    add_ok_count = cJSON_AddItemToObject(root, "count", count_num);

    // step 5: Validate & Cleanup
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
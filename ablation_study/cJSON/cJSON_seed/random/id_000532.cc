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
//<ID> 532
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *double_arr = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_name = (cJSON *)0;
    cJSON *retrieved_elem = (cJSON *)0;
    double numbers[3];
    int num_count = 3;
    cJSON_bool add_ok_arr = 0;
    cJSON_bool add_ok_obj1 = 0;
    cJSON_bool add_ok_obj2 = 0;
    cJSON_bool is_raw = 0;
    char *name_val = (char *)0;

    // step 2: Initialize
    numbers[0] = 1.1;
    numbers[1] = 2.2;
    numbers[2] = 3.3;
    root = cJSON_CreateObject();
    double_arr = cJSON_CreateDoubleArray(numbers, num_count);
    raw_item = cJSON_CreateRaw("RAW_PAYLOAD");
    name_item = cJSON_CreateString("example_name");

    // step 3: Configure
    add_ok_arr = cJSON_AddItemToArray(double_arr, raw_item);
    add_ok_obj1 = cJSON_AddItemToObject(root, "values", double_arr);
    add_ok_obj2 = cJSON_AddItemToObject(root, "name", name_item);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "values");
    retrieved_elem = cJSON_GetArrayItem(retrieved_arr, 3);
    is_raw = cJSON_IsRaw(retrieved_elem);
    retrieved_name = cJSON_GetObjectItem(root, "name");
    name_val = cJSON_GetStringValue(retrieved_name);

    // step 5: Validate
    (void)add_ok_arr;
    (void)add_ok_obj1;
    (void)add_ok_obj2;
    (void)is_raw;
    (void)name_val;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
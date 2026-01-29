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
//<ID> 531
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *outer_arr = (cJSON *)0;
    cJSON *double_arr = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    const double numbers[] = {1.1, 2.2, 3.3};
    int num_count = 3;
    cJSON_bool add_ok_array = 0;
    cJSON_bool add_ok_object = 0;
    cJSON_bool is_raw = 0;
    char *name_val = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    outer_arr = cJSON_CreateArray();
    double_arr = cJSON_CreateDoubleArray(numbers, num_count);
    name_item = cJSON_CreateString("example_name");

    // step 3: Configure
    add_ok_array = cJSON_AddItemToArray(outer_arr, double_arr);
    add_ok_object = cJSON_AddItemToObject(root, "dataset", outer_arr);
    cJSON_AddItemToObject(root, "name", name_item);

    // step 4: Operate
    name_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    is_raw = cJSON_IsRaw(double_arr);

    // step 5: Validate
    (void)add_ok_array;
    (void)add_ok_object;
    (void)name_val;
    (void)is_raw;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
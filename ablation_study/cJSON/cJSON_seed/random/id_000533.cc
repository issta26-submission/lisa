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
//<ID> 533
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = nullptr;
    cJSON *arr = nullptr;
    cJSON *dbl_arr = nullptr;
    cJSON *raw_item = nullptr;
    cJSON *retrieved_arr = nullptr;
    const double numbers[] = {1.5, 2.5, 3.5};
    int num_count = 3;
    cJSON_bool add_ok_array = 0;
    cJSON_bool add_ok_object = 0;
    cJSON_bool raw_check = 0;
    char *raw_value = nullptr;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    dbl_arr = cJSON_CreateDoubleArray(numbers, num_count);
    raw_item = cJSON_CreateRaw("{\"note\":\"raw payload\"}");

    // step 3: Configure
    add_ok_array = cJSON_AddItemToArray(arr, dbl_arr);
    add_ok_object = cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToObject(root, "meta", raw_item);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "data");
    raw_check = cJSON_IsRaw(raw_item);
    raw_value = cJSON_GetStringValue(raw_item);

    // step 5: Validate
    (void)add_ok_array;
    (void)add_ok_object;
    (void)retrieved_arr;
    (void)raw_check;
    (void)raw_value;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
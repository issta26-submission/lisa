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
//<ID> 709
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_bool added_arr = cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON_bool add_str_ok = cJSON_AddItemToArray(arr, str_item);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_bool add_true_ok = cJSON_AddItemToArray(arr, true_item);
    cJSON *num_item = cJSON_AddNumberToObject(root, "pi", 3.1415);

    // step 3: Operate
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *first_elem = cJSON_GetArrayItem(arr, 0);
    const char *first_str = cJSON_GetStringValue(first_elem);
    cJSON *got_num = cJSON_GetObjectItem(root, "pi");
    double got_num_val = cJSON_GetNumberValue(got_num);

    // step 4: Validate
    int validation = 0;
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(added_arr != 0);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(add_str_ok != 0);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(add_true_ok != 0);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(arr_size == 2);
    validation ^= (int)(first_str != NULL && first_str[0] == 'h');
    validation ^= (int)(got_num_val == 3.1415);
    (void)validation;

    // step 5: Cleanup
    char *serialized = cJSON_PrintUnformatted(root);
    cJSON_free((void*)serialized);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
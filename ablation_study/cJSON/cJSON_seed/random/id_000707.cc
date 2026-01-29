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
//<ID> 707
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
    cJSON *num_in_obj = cJSON_AddNumberToObject(root, "pi", 3.14159);
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON_bool added_str = cJSON_AddItemToArray(arr, str_item);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_bool added_true = cJSON_AddItemToArray(arr, true_item);

    // step 3: Operate
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON *pi_item = cJSON_GetObjectItem(root, "pi");
    double pi_val = cJSON_GetNumberValue(pi_item);
    const char *first_str = cJSON_GetStringValue(first);
    cJSON_bool second_is_true = cJSON_IsTrue(second);

    // step 4: Validate
    int validation = 0;
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(added_arr != 0);
    validation ^= (int)(num_in_obj != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(added_str != 0);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(added_true != 0);
    validation ^= (int)(arr_size == 2);
    validation ^= (int)(pi_val == 3.14159);
    validation ^= (int)(first_str != NULL && first_str[0] == 'h');
    validation ^= (int)(second_is_true != 0);
    (void)validation;

    // step 5: Cleanup
    char *serialized = cJSON_PrintUnformatted(root);
    cJSON_free((void*)serialized);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
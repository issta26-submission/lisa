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
//<ID> 43
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *initial = cJSON_AddNumberToObject(root, "initial", 1.5);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_InsertItemInArray(arr, 0, n1);
    cJSON_InsertItemInArray(arr, 1, n2);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "is_valid", true_item);
    cJSON_AddItemToObject(root, "is_empty", false_item);

    // step 2: Configure
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", 0.0);

    // step 3: Operate and Validate
    int arr_size = cJSON_GetArraySize(arr);
    cJSON_SetNumberHelper(count_item, (double)arr_size);
    cJSON *first_elem = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first_elem);
    double init_val = cJSON_GetNumberValue(initial);
    cJSON_bool valid_flag = cJSON_IsTrue(cJSON_GetObjectItemCaseSensitive(root, "is_valid"));
    cJSON_bool empty_flag = cJSON_IsFalse(cJSON_GetObjectItem(root, "is_empty"));
    int summary = (int)(first_val + init_val) + arr_size + (int)valid_flag + (int)empty_flag;
    (void)summary;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
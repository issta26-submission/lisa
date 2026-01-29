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
//<ID> 1743
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num = (cJSON *)0;
    cJSON *str0 = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    int arr_size = 0;
    cJSON_bool num_is_number = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num = cJSON_CreateNumber(42.0);
    str0 = cJSON_CreateString("first");
    str1 = cJSON_CreateString("second");

    // step 3: Configure
    cJSON_AddItemToArray(arr, str0);
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "num", num);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "values");
    retrieved_num = cJSON_GetObjectItemCaseSensitive(root, "num");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    num_is_number = cJSON_IsNumber(retrieved_num);

    // step 5: Validate
    validation = (int)(arr_size == 2) + (int)(num_is_number != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)num;
    (void)str0;
    (void)str1;
    (void)retrieved_arr;
    (void)retrieved_num;
    (void)arr_size;
    (void)num_is_number;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1741
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
    cJSON *num_elem = (cJSON *)0;
    cJSON *str_elem = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_count = (cJSON *)0;
    int arr_size = 0;
    cJSON_bool count_is_number = 0;
    char *extracted_str = (char *)0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_elem = cJSON_CreateNumber(42.0);
    str_elem = cJSON_CreateString("payload");

    // step 3: Configure
    cJSON_AddItemToArray(arr, num_elem);
    cJSON_AddItemToArray(arr, str_elem);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToObject(root, "count", cJSON_CreateNumber(2));

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "data");
    retrieved_count = cJSON_GetObjectItemCaseSensitive(root, "count");
    count_is_number = cJSON_IsNumber(retrieved_count);
    arr_size = cJSON_GetArraySize(retrieved_arr);
    extracted_str = cJSON_GetStringValue(str_elem);

    // step 5: Validate
    validation = (int)(arr_size == 2) + (int)(count_is_number != 0) + (int)(extracted_str != (char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)num_elem;
    (void)str_elem;
    (void)retrieved_arr;
    (void)retrieved_count;
    (void)arr_size;
    (void)count_is_number;
    (void)extracted_str;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1747
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
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON *num = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    char *first_str = (char *)0;
    int arr_size = 0;
    cJSON_bool num_is_number = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("apple");
    s2 = cJSON_CreateString("banana");
    num = cJSON_CreateNumber(123.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "fruits", arr);
    cJSON_AddItemToObject(root, "count", num);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "fruits");
    retrieved_num = cJSON_GetObjectItemCaseSensitive(root, "count");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    num_is_number = cJSON_IsNumber(retrieved_num);
    first_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    first_str = cJSON_GetStringValue(first_elem);

    // step 5: Validate
    validation = (int)(arr_size == 2) + (int)(num_is_number != 0) + (int)(first_str != (char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)s1;
    (void)s2;
    (void)num;
    (void)retrieved_arr;
    (void)retrieved_num;
    (void)first_elem;
    (void)first_str;
    (void)arr_size;
    (void)num_is_number;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}
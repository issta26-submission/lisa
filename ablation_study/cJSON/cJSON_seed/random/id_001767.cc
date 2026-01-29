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
//<ID> 1767
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
    cJSON *s0 = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *count_item = (cJSON *)0;
    cJSON *retrieved_list = (cJSON *)0;
    cJSON *retrieved_count = (cJSON *)0;
    int arr_size = 0;
    double count_value = 0.0;
    cJSON_bool is_array = 0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s0 = cJSON_CreateString("alpha");
    s1 = cJSON_CreateString("beta");
    count_item = cJSON_CreateNumber(2.0);

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, s0);
    add_ok2 = cJSON_AddItemToArray(arr, s1);
    add_ok3 = cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToObject(root, "count", count_item);

    // step 4: Operate
    retrieved_list = cJSON_GetObjectItem(root, "list");
    is_array = cJSON_IsArray(retrieved_list);
    arr_size = cJSON_GetArraySize(retrieved_list);
    retrieved_count = cJSON_GetObjectItem(root, "count");
    count_value = cJSON_GetNumberValue(retrieved_count);

    // step 5: Validate / modify structure
    validation = (int)(arr_size == 2) + (int)(is_array != 0) + (int)(add_ok1 != 0) + (int)(add_ok2 != 0) + (int)(add_ok3 != 0) + (int)(count_value == 2.0);
    cJSON_DeleteItemFromObject(root, "count");
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)s0;
    (void)s1;
    (void)count_item;
    (void)retrieved_list;
    (void)retrieved_count;
    (void)arr_size;
    (void)count_value;
    (void)is_array;
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}
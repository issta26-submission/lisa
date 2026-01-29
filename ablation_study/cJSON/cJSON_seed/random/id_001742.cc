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
//<ID> 1742
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
    cJSON *count_item = (cJSON *)0;
    int arr_size = 0;
    cJSON_bool is_num = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("one");
    s2 = cJSON_CreateString("two");
    num = cJSON_CreateNumber(2.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "count", num);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    count_item = cJSON_GetObjectItemCaseSensitive(root, "count");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    is_num = cJSON_IsNumber(count_item);

    // step 5: Validate
    validation = (int)(arr_size == 2) + (int)(is_num != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)s1;
    (void)s2;
    (void)num;
    (void)retrieved_arr;
    (void)count_item;
    (void)arr_size;
    (void)is_num;
    (void)validation;

    // API sequence test completed successfully
    return 66;
}
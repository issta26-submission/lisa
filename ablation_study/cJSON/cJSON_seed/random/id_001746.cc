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
//<ID> 1746
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
    cJSON *s2 = (cJSON *)0;
    cJSON *num = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_count = (cJSON *)0;
    char *printed = (char *)0;
    int arr_size = 0;
    cJSON_bool is_num = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s0 = cJSON_CreateString("one");
    s1 = cJSON_CreateString("two");
    s2 = cJSON_CreateString("three");
    num = cJSON_CreateNumber(3.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, s0);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "count", num);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    retrieved_count = cJSON_GetObjectItemCaseSensitive(root, "count");
    is_num = cJSON_IsNumber(retrieved_count);
    arr_size = cJSON_GetArraySize(retrieved_arr);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation = (int)(arr_size == 3) + (int)(is_num != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)s0;
    (void)s1;
    (void)s2;
    (void)num;
    (void)retrieved_arr;
    (void)retrieved_count;
    (void)arr_size;
    (void)is_num;
    (void)printed;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}
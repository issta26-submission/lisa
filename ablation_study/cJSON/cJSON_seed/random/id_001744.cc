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
//<ID> 1744
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
    cJSON *strItem = (cJSON *)0;
    cJSON *numItem = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_arr_cs = (cJSON *)0;
    cJSON *first = (cJSON *)0;
    cJSON *second = (cJSON *)0;
    char *first_str = (char *)0;
    cJSON_bool second_is_num = 0;
    int arr_size = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    strItem = cJSON_CreateString("hello");
    numItem = cJSON_CreateNumber(123.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, strItem);
    cJSON_AddItemToArray(arr, numItem);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "label", cJSON_CreateString("example_label"));

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    retrieved_arr_cs = cJSON_GetObjectItemCaseSensitive(root, "items");
    first = cJSON_GetArrayItem(retrieved_arr, 0);
    second = cJSON_GetArrayItem(retrieved_arr_cs, 1);
    first_str = cJSON_GetStringValue(first);
    second_is_num = cJSON_IsNumber(second);
    arr_size = cJSON_GetArraySize(retrieved_arr);

    // step 5: Validate
    validation = (int)(arr_size == 2) + (int)(first_str != (char *)0) + (int)(second_is_num != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)strItem;
    (void)numItem;
    (void)retrieved_arr;
    (void)retrieved_arr_cs;
    (void)first;
    (void)second;
    (void)first_str;
    (void)second_is_num;
    (void)arr_size;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}
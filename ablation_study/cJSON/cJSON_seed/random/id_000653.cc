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
//<ID> 653
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
    cJSON *str0 = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *retrieved0 = (cJSON *)0;
    cJSON *retrieved1 = (cJSON *)0;
    cJSON_bool added_to_arr0 = 0;
    cJSON_bool added_to_arr1 = 0;
    cJSON_bool added_to_obj = 0;
    int array_size = 0;
    char *val0 = (char *)0;
    char *val1 = (char *)0;
    int pointers_equal = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str0 = cJSON_CreateString("hello");
    str1 = cJSON_CreateString("world");

    // step 3: Configure
    added_to_arr0 = cJSON_AddItemToArray(arr, str0);
    added_to_arr1 = cJSON_AddItemToArray(arr, str1);
    added_to_obj = cJSON_AddItemToObject(root, "greetings", arr);

    // step 4: Operate
    array_size = cJSON_GetArraySize(arr);
    retrieved0 = cJSON_GetArrayItem(arr, 0);
    retrieved1 = cJSON_GetArrayItem(arr, 1);
    val0 = cJSON_GetStringValue(retrieved0);
    val1 = cJSON_GetStringValue(retrieved1);

    // step 5: Validate
    pointers_equal = (val0 == val1);
    (void)added_to_arr0;
    (void)added_to_arr1;
    (void)added_to_obj;
    (void)array_size;
    (void)retrieved0;
    (void)retrieved1;
    (void)val0;
    (void)val1;
    (void)pointers_equal;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1254
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
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *str_child = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON_bool is_str = 0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Initialize - create container objects and items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(3.14);
    num2 = cJSON_CreateNumber(42.0);
    str_child = cJSON_CreateString("hello");

    // step 3: Configure - assemble the JSON structure and a reference array
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "numbers", arr);
    arr_ref = cJSON_CreateArrayReference(str_child);
    cJSON_AddItemToObject(root, "refs", arr_ref);

    // step 4: Operate - inspect types and measure array size
    is_str = cJSON_IsString(str_child);
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate - compute a small validation score to ensure flow
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(num1 != (cJSON *)0) + (int)(num2 != (cJSON *)0) + (int)(str_child != (cJSON *)0) + (int)is_str + arr_size;
    (void)validation_score;

    // step 6: Cleanup - delete root (frees arr, num1, num2, arr_ref) and free the referenced child
    cJSON_Delete(root);
    cJSON_Delete(str_child);

    // API sequence test completed successfully
    return 66;
}
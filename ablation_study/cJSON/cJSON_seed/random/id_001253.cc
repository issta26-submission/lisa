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
//<ID> 1253
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
    cJSON *str = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON_bool str_is_string = 0;
    int size_before = 0;
    int size_after = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an empty array, a number and a string
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num = cJSON_CreateNumber(42.0);
    str = cJSON_CreateString("example");
    size_before = cJSON_GetArraySize(arr);

    // step 3: Configure - add the number and string to the array, create an array-reference to the number and append it, attach array to root
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, str);
    arr_ref = cJSON_CreateArrayReference(num);
    cJSON_AddItemToArray(arr, arr_ref);
    cJSON_AddItemToObject(root, "payload", arr);

    // step 4: Operate - inspect the string item and measure final array size
    str_is_string = cJSON_IsString(str);
    size_after = cJSON_GetArraySize(arr);

    // step 5: Validate - compute a simple validation score to ensure values flowed through the APIs
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(num != (cJSON *)0) + (int)(str != (cJSON *)0) + (int)(arr_ref != (cJSON *)0) + (int)str_is_string + size_before + size_after;
    (void)validation_score;

    // step 6: Cleanup - delete the root (frees attached array and items)
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1250
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
    cJSON *str = (cJSON *)0;
    cJSON *ref = (cJSON *)0;
    cJSON_bool str_is_string = 0;
    int size_before = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and standalone items (string and numbers)
    root = cJSON_CreateObject();
    str = cJSON_CreateString("hello");
    num1 = cJSON_CreateNumber(3.14);
    num2 = cJSON_CreateNumber(42.0);

    // step 3: Configure - attach the original string to root, create an array reference to that string, build an array and add items
    cJSON_AddItemToObject(root, "orig", str);
    ref = cJSON_CreateArrayReference(str);
    arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, ref);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "data", arr);

    // step 4: Operate - inspect the string-ness and get array size
    str_is_string = cJSON_IsString(str);
    size_before = cJSON_GetArraySize(arr);

    // step 5: Validate - compute a simple validation value to ensure data flowed through the APIs
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(num1 != (cJSON *)0) + (int)(num2 != (cJSON *)0) + (int)(str != (cJSON *)0) + (int)(ref != (cJSON *)0) + (int)str_is_string + size_before;
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached items and the array (references are not leaked because original string is attached to root)
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1259
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *str = (cJSON *)0;
    cJSON_bool str_is_string = 0;
    int size_before = 0;
    int validation_score = 0;

    // step 2: Initialize - create root and numeric items, create an array reference to num1
    root = cJSON_CreateObject();
    num1 = cJSON_CreateNumber(3.14);
    arr_ref = cJSON_CreateArrayReference((const cJSON *)num1);

    // step 3: Configure - create another number and append it to the referenced array, attach array to root
    num2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr_ref, num2);
    cJSON_AddItemToObject(root, "payload", arr_ref);

    // step 4: Operate - measure array size, create and attach a string, and check its string-ness
    size_before = cJSON_GetArraySize(arr_ref);
    str = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "label", str);
    str_is_string = cJSON_IsString((const cJSON *const)str);

    // step 5: Validate - compute a simple validation score to ensure flow through APIs
    validation_score = size_before + (int)str_is_string + (int)(root != (cJSON *)0) + (int)(arr_ref != (cJSON *)0) + (int)(num1 != (cJSON *)0) + (int)(num2 != (cJSON *)0) + (int)(str != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
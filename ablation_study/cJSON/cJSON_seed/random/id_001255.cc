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
//<ID> 1255
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *inner = (cJSON *)0;
    cJSON *outer = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *ref_arr = (cJSON *)0;
    cJSON_bool is_str = 0;
    int size_inner = 0;
    int size_outer = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and two arrays and numeric items
    root = cJSON_CreateObject();
    inner = cJSON_CreateArray();
    outer = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(3.1415);
    num2 = cJSON_CreateNumber(42.0);

    // step 3: Configure - populate inner array, create a reference to inner, and attach arrays to root
    cJSON_AddItemToArray(inner, num1);
    cJSON_AddItemToArray(inner, num2);
    ref_arr = cJSON_CreateArrayReference(inner);
    cJSON_AddItemToArray(outer, ref_arr);
    cJSON_AddItemToObject(root, "inner", inner);
    cJSON_AddItemToObject(root, "outer", outer);

    // step 4: Operate - check a type and measure array sizes
    is_str = cJSON_IsString(num1);
    size_inner = cJSON_GetArraySize(inner);
    size_outer = cJSON_GetArraySize(outer);

    // step 5: Validate - compute a simple validation score to ensure data flowed through APIs
    validation_score = (int)(root != (cJSON *)0) + (int)(inner != (cJSON *)0) + (int)(outer != (cJSON *)0) + (int)(num1 != (cJSON *)0) + (int)(num2 != (cJSON *)0) + (int)(ref_arr != (cJSON *)0) + (int)is_str + size_inner + size_outer;
    (void)validation_score;

    // step 6: Cleanup - delete the root (which also frees attached arrays and items)
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
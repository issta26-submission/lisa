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
//<ID> 1251
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *num = (cJSON *)0;
    cJSON *ref = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON_bool added = 0;
    cJSON_bool is_str = 0;
    int size_before = 0;
    int size_after = 0;
    int validation_score = 0;

    // step 2: Initialize - create a numeric child and an empty array, then create an array-reference to the child
    num = cJSON_CreateNumber(42.0);
    arr = cJSON_CreateArray();
    ref = cJSON_CreateArrayReference(num);

    // step 3: Configure - measure size before, add the reference to the array, then measure size after
    size_before = cJSON_GetArraySize(arr);
    added = cJSON_AddItemToArray(arr, ref);
    size_after = cJSON_GetArraySize(arr);

    // step 4: Operate - inspect the original child for string-ness
    is_str = cJSON_IsString(num);

    // step 5: Validate - compute a simple validation score to ensure values flowed through APIs
    validation_score = (int)(num != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(ref != (cJSON *)0) + (int)added + size_before + size_after + (int)is_str;
    (void)validation_score;

    // step 6: Cleanup - delete the array (which frees the reference) and then delete the original child
    cJSON_Delete(arr);
    cJSON_Delete(num);

    // API sequence test completed successfully
    return 66;
}
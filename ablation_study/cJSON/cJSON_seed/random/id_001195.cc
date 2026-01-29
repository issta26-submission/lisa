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
//<ID> 1195
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *arr = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *str2 = (cJSON *)0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool are_equal = 0;
    int size = 0;
    int validation_score = 0;

    // step 2: Setup - create an array to hold string items
    arr = cJSON_CreateArray();

    // step 3: Configure - create two identical string items and add them to the array
    str1 = cJSON_CreateString("fuzz_string");
    str2 = cJSON_CreateString("fuzz_string");
    added1 = cJSON_AddItemToArray(arr, str1);
    added2 = cJSON_AddItemToArray(arr, str2);

    // step 4: Operate - compare the two string items for equality (case-sensitive)
    are_equal = cJSON_Compare((const cJSON *const)str1, (const cJSON *const)str2, (cJSON_bool)1);

    // step 5: Validate - compute a small validation score using comparison result and array size
    size = cJSON_GetArraySize(arr);
    validation_score = (int)are_equal + size + (int)added1 + (int)added2;

    // step 6: Cleanup - delete the array which also frees its child items
    cJSON_Delete(arr);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}
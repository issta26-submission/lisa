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
//<ID> 1191
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
    cJSON_bool are_equal = 0;
    int validation_score = 0;

    // step 2: Setup - create an array container
    arr = cJSON_CreateArray();

    // step 3: Configure - create two string items with identical content
    str1 = cJSON_CreateString("fuzz_string");
    str2 = cJSON_CreateString("fuzz_string");

    // step 4: Operate - add both string items to the array
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, str2);

    // step 5: Validate - compare the two items and compute a small validation score
    are_equal = cJSON_Compare((const cJSON *const)str1, (const cJSON *const)str2, (cJSON_bool)1);
    validation_score = (int)are_equal + cJSON_GetArraySize(arr);

    // step 6: Cleanup - delete the array which frees attached items
    cJSON_Delete(arr);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}
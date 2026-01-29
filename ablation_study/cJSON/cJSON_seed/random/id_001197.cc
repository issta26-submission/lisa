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
//<ID> 1197
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *arr = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *str2 = (cJSON *)0;
    cJSON *str3 = (cJSON *)0;
    cJSON_bool same12 = 0;
    cJSON_bool same13 = 0;
    int validation_score = 0;

    // step 2: Setup - create an array and string items
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("hello");
    str2 = cJSON_CreateString("hello");
    str3 = cJSON_CreateString("world");

    // step 3: Configure - add the string items to the array
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, str2);
    cJSON_AddItemToArray(arr, str3);

    // step 4: Operate - compare elements and compute a validation score
    same12 = cJSON_Compare((const cJSON *const)str1, (const cJSON *const)str2, (cJSON_bool)1);
    same13 = cJSON_Compare((const cJSON *const)str1, (const cJSON *const)str3, (cJSON_bool)1);
    validation_score = (int)same12 + (int)(!same13) + cJSON_GetArraySize(arr);

    // step 5: Cleanup - delete the array which frees attached items
    cJSON_Delete(arr);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}
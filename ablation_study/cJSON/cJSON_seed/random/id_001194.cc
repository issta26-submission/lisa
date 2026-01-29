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
//<ID> 1194
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *arr = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON_bool add_res1 = 0;
    cJSON_bool add_res2 = 0;
    cJSON_bool cmp_result = 0;
    int validation_score = 0;

    // step 2: Setup - create an array and two string items
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("example");
    s2 = cJSON_CreateString("example");

    // step 3: Configure - add the string items to the array
    add_res1 = cJSON_AddItemToArray(arr, s1);
    add_res2 = cJSON_AddItemToArray(arr, s2);

    // step 4: Operate - compare the two array items case-sensitively
    cmp_result = cJSON_Compare(arr->child, arr->child->next, 1);

    // step 5: Validate & Cleanup - compute a small validation score and free resources
    validation_score = (int)cmp_result + (int)cJSON_GetArraySize(arr);
    cJSON_Delete(arr);

    // API sequence test completed successfully
    (void)validation_score;
    (void)add_res1;
    (void)add_res2;
    (void)cmp_result;
    return 66;
}
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
//<ID> 1190
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *arr = (cJSON *)0;
    cJSON *arr2 = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool equal = 0;
    int validation_score = 0;

    // step 2: Setup - create two arrays and two string items
    arr = cJSON_CreateArray();
    arr2 = cJSON_CreateArray();
    s1 = cJSON_CreateString("shared_value");
    s2 = cJSON_CreateString("shared_value");

    // step 3: Configure - add each string to its respective array
    added1 = cJSON_AddItemToArray(arr, s1);
    added2 = cJSON_AddItemToArray(arr2, s2);

    // step 4: Operate - compare the two arrays case-sensitively
    equal = cJSON_Compare(arr, arr2, (cJSON_bool)1);

    // step 5: Validate - compute a small validation score from results
    validation_score = (int)equal + (int)added1 + (int)added2;

    // step 6: Cleanup - delete arrays which also frees contained items
    cJSON_Delete(arr);
    cJSON_Delete(arr2);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}
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
//<ID> 900
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "flags";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *flag0 = (cJSON *)0;
    cJSON *flag1 = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON_bool added_arr = (cJSON_bool)0;
    cJSON_bool is_false = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure - create false flags and add them to the array, then attach the array to the root object
    flag0 = cJSON_CreateFalse();
    flag1 = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, flag0);
    cJSON_AddItemToArray(arr, flag1);
    added_arr = cJSON_AddItemToObject(root, arr_key, arr);

    // step 4: Operate / Validate - retrieve the second array item and check that it is false
    retrieved = cJSON_GetArrayItem(arr, 1);
    is_false = cJSON_IsFalse(retrieved);
    validation_score = (int)(root != (cJSON *)0) + (int)added_arr + (int)(retrieved != (cJSON *)0) + (int)is_false;
    (void)validation_score;

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
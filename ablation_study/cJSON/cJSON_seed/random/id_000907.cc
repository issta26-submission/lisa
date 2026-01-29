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
//<ID> 907
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *array_key = "items";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON_bool added_array = (cJSON_bool)0;
    cJSON_bool first_is_false = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(7.0));

    // step 3: Configure - attach array to root
    added_array = cJSON_AddItemToObject(root, array_key, arr);

    // step 4: Operate - retrieve first array element and check if it is false
    first_elem = cJSON_GetArrayItem(arr, 0);
    first_is_false = cJSON_IsFalse(first_elem);

    // step 5: Validate - aggregate simple checks
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)added_array + (int)(first_elem != (cJSON *)0) + (int)first_is_false;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
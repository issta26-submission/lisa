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
//<ID> 902
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *array_key = "items";
    const char *sval = "example";
    double nval = 7.0;
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *retrieved0 = (cJSON *)0;
    cJSON *retrieved1 = (cJSON *)0;
    cJSON_bool added_array = (cJSON_bool)0;
    cJSON_bool is_false = (cJSON_bool)0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    false_item = cJSON_CreateFalse();
    num_item = cJSON_CreateNumber(nval);
    str_item = cJSON_CreateString(sval);

    // step 3: Configure - populate array and attach to root
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, str_item);
    added_array = cJSON_AddItemToObject(root, array_key, arr);

    // step 4: Operate / Validate - retrieve array items and check boolean
    retrieved0 = cJSON_GetArrayItem(arr, 0);
    retrieved1 = cJSON_GetArrayItem(arr, 1);
    is_false = cJSON_IsFalse(retrieved0);
    printed = cJSON_Print(root);
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(false_item != (cJSON *)0) + (int)added_array + (int)is_false + (int)(retrieved1 != (cJSON *)0) + (int)(printed != (char *)0);

    // step 5: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}
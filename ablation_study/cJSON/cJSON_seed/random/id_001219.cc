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
//<ID> 1219
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *arr = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    cJSON *second_item = (cJSON *)0;
    char *str_value = (char *)0;
    int array_size = 0;
    int validation_score = 0;

    // step 2: Setup - create an array and add a string and a boolean item
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("fuzz_label");
    cJSON_AddItemToArray(arr, str_item);
    bool_item = cJSON_CreateBool(1);
    cJSON_AddItemToArray(arr, bool_item);

    // step 3: Operate - obtain array size and retrieve both elements
    array_size = cJSON_GetArraySize(arr);
    first_item = cJSON_GetArrayItem(arr, 0);
    second_item = cJSON_GetArrayItem(arr, 1);
    str_value = cJSON_GetStringValue(first_item);

    // step 4: Validate - compute a small validation score from meaningful data flow
    validation_score = array_size + (int)(str_value != (char *)0) + (int)(first_item != (cJSON *)0) + (int)(second_item != (cJSON *)0) + (int)(bool_item != (cJSON *)0);

    // step 5: Cleanup - delete the array which frees all attached items
    cJSON_Delete(arr);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}
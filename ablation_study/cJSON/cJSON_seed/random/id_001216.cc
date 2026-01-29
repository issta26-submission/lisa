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
//<ID> 1216
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *s0 = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *b0 = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    char *str_from_item0 = (char *)0;
    char *str_from_item1 = (char *)0;
    int array_size = 0;
    int validation_score = 0;

    // step 2: Setup - create root object and an array, attach array to root
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Configure - create string items and a boolean item, then add them to the array
    s0 = cJSON_CreateString("element_zero");
    b0 = cJSON_CreateBool(1);
    s1 = cJSON_CreateString("element_two");
    cJSON_AddItemToArray(arr, s0);
    cJSON_AddItemToArray(arr, b0);
    cJSON_AddItemToArray(arr, s1);

    // step 4: Operate - obtain array size and read items at specific positions
    array_size = cJSON_GetArraySize(arr);
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);
    str_from_item0 = cJSON_GetStringValue(item0);
    str_from_item1 = cJSON_GetStringValue(item1);

    // step 5: Validate - compute a small validation score from the data flow
    validation_score = array_size + (int)(str_from_item0 != (char *)0) + (int)(str_from_item1 != (char *)0);

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}
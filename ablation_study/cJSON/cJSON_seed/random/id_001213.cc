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
//<ID> 1213
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *arr = (cJSON *)0;
    cJSON *b_true = (cJSON *)0;
    cJSON *b_false = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *item2 = (cJSON *)0;
    int array_size = 0;
    char *str_val = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create an array container
    arr = cJSON_CreateArray();

    // step 3: Configure - create boolean and string items and add them to the array
    b_true = cJSON_CreateBool(1);
    b_false = cJSON_CreateBool(0);
    str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, b_true);
    cJSON_AddItemToArray(arr, b_false);
    cJSON_AddItemToArray(arr, str_item);

    // step 4: Operate - retrieve array size and items by index
    array_size = cJSON_GetArraySize(arr);
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);
    item2 = cJSON_GetArrayItem(arr, 2);
    str_val = cJSON_GetStringValue(item2);

    // step 5: Validate - compute a small validation score from meaningful data flow
    validation_score = array_size + (int)(str_val != (char *)0) + (int)(cJSON_IsTrue(item0));

    // step 6: Cleanup - delete the array which frees all attached items
    cJSON_Delete(arr);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}
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
//<ID> 1211
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
    cJSON *b1 = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    char *str0 = (char *)0;
    int size = 0;
    int validation_score = 0;

    // step 2: Setup - create an array and two items (string + boolean)
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("hello");
    b1 = cJSON_CreateBool((cJSON_bool)1);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, b1);

    // step 3: Operate - obtain array size and retrieve both items
    size = cJSON_GetArraySize(arr);
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);

    // step 4: Validate - read string value from first item and compute a small score
    str0 = cJSON_GetStringValue(item0);
    validation_score = size + (int)(str0 != (char *)0) + (int)(item1 != (cJSON *)0);

    // step 5: Cleanup - delete the array which frees attached items
    cJSON_Delete(arr);

    // step 6: Finalize - silence unused local and finish
    (void)validation_score;

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1199
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
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON_bool are_equal = 0;
    int size = 0;
    int validation_score = 0;

    // step 2: Setup - create an array container
    arr = cJSON_CreateArray();

    // step 3: Configure - create two string items with identical contents and add them to the array
    s1 = cJSON_CreateString("alpha");
    s2 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);

    // step 4: Operate - retrieve the items and compare them for equality (case-sensitive)
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);
    are_equal = cJSON_Compare(item0, item1, 1);

    // step 5: Validate - obtain array size and compute a small validation score
    size = cJSON_GetArraySize(arr);
    validation_score = (int)are_equal + size;

    // step 6: Cleanup - delete the array which frees all attached items
    cJSON_Delete(arr);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}
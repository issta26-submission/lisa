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
//<ID> 1210
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *arr = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    int size = 0;
    const char *svalue = (const char *)0;
    int validation_score = 0;

    // step 2: Setup - create an array and populate it with a string and boolean values
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("test_string");
    true_item = cJSON_CreateBool(1);
    false_item = cJSON_CreateBool(0);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToArray(arr, false_item);

    // step 3: Operate - obtain array size and retrieve elements by index
    size = cJSON_GetArraySize(arr);
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);
    svalue = cJSON_GetStringValue(item0);

    // step 4: Validate - compute a small validation score based on meaningful data flow
    validation_score = size + (int)(svalue != (const char *)0) + (int)cJSON_IsTrue(item1);

    // step 5: Cleanup - delete the array which frees all attached items
    cJSON_Delete(arr);

    // step 6: Finalize - use validation_score to avoid unused warnings
    (void)validation_score;

    // API sequence test completed successfully
    return 66;
}
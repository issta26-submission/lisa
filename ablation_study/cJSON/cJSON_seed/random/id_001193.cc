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
//<ID> 1193
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *arr = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON *fetched0 = (cJSON *)0;
    cJSON *fetched1 = (cJSON *)0;
    char *serialized = (char *)0;
    cJSON_bool are_equal = 0;
    int validation_score = 0;

    // step 2: Setup - create an array container
    arr = cJSON_CreateArray();

    // step 3: Configure - create two identical string items and add them to the array
    s1 = cJSON_CreateString("test_string");
    s2 = cJSON_CreateString("test_string");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);

    // step 4: Operate - retrieve the items and compare them for equality (case sensitive)
    fetched0 = cJSON_GetArrayItem(arr, 0);
    fetched1 = cJSON_GetArrayItem(arr, 1);
    are_equal = cJSON_Compare(fetched0, fetched1, 1);

    // step 5: Validate - compute a small validation score and serialize the array for sanity
    validation_score = (int)are_equal + cJSON_GetArraySize(arr);
    serialized = cJSON_PrintUnformatted(arr);
    cJSON_free((void *)serialized);

    // step 6: Cleanup - delete the array which frees all attached items
    cJSON_Delete(arr);

    // API sequence test completed successfully
    (void)validation_score;
    (void)fetched0;
    (void)fetched1;
    return 66;
}
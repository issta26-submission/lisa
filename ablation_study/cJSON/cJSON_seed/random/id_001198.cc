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
//<ID> 1198
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
    cJSON *s3 = (cJSON *)0;
    cJSON *f0 = (cJSON *)0;
    cJSON *f1 = (cJSON *)0;
    cJSON_bool eq_same = 0;
    cJSON_bool eq_diff = 0;
    int validation_score = 0;

    // step 2: Setup - create an array and string items
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("alpha");
    s2 = cJSON_CreateString("beta");

    // step 3: Configure - add string items to the array
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);

    // step 4: Operate / Validate - fetch items and compare
    f0 = cJSON_GetArrayItem(arr, 0);
    f1 = cJSON_GetArrayItem(arr, 1);
    s3 = cJSON_CreateString("alpha");
    eq_same = cJSON_Compare(f0, s3, 1);
    eq_diff = cJSON_Compare(f0, f1, 1);
    validation_score = (int)eq_same + (int)eq_diff + cJSON_GetArraySize(arr);

    // step 5: Cleanup - delete separately created item then delete array (which frees its children)
    cJSON_Delete(s3);
    cJSON_Delete(arr);

    (void)validation_score;
    // API sequence test completed successfully
    return 66;
}
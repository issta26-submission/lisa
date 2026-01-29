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
//<ID> 1192
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *arr = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *str2 = (cJSON *)0;
    cJSON *f0 = (cJSON *)0;
    cJSON *f1 = (cJSON *)0;
    cJSON_bool add_result1 = 0;
    cJSON_bool add_result2 = 0;
    cJSON_bool cmp_case_sensitive = 0;
    cJSON_bool cmp_case_insensitive = 0;
    int arr_size = 0;
    int validation_score = 0;
    char *serialized = (char *)0;

    // step 2: Setup - create array and string items
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("example");
    str2 = cJSON_CreateString("example");

    // step 3: Configure - add strings to the array
    add_result1 = cJSON_AddItemToArray(arr, str1);
    add_result2 = cJSON_AddItemToArray(arr, str2);

    // step 4: Operate - fetch items and compare them with different case sensitivity flags
    f0 = cJSON_GetArrayItem(arr, 0);
    f1 = cJSON_GetArrayItem(arr, 1);
    cmp_case_sensitive = cJSON_Compare((const cJSON *const)f0, (const cJSON *const)f1, 1);
    cmp_case_insensitive = cJSON_Compare((const cJSON *const)f0, (const cJSON *const)f1, 0);
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate - compute a simple validation score and exercise printing API then free buffer
    validation_score = (int)add_result1 + (int)add_result2 + (int)cmp_case_sensitive + (int)cmp_case_insensitive + arr_size;
    serialized = cJSON_PrintUnformatted(arr);
    cJSON_free((void *)serialized);

    // step 6: Cleanup - delete array which frees contained items
    cJSON_Delete(arr);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}
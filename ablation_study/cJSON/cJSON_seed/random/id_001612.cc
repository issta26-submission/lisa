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
//<ID> 1612
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *n3 = (cJSON *)0;
    cJSON_bool added_arr = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool added3 = 0;
    int sz_before = 0;
    int sz_after = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(10.0);
    n2 = cJSON_CreateNumber(20.0);
    n3 = cJSON_CreateNumber(30.0);

    // step 3: Configure
    added_arr = cJSON_AddItemToObject(root, "values", arr);
    added1 = cJSON_AddItemToArray(arr, n1);
    added2 = cJSON_AddItemToArray(arr, n2);
    added3 = cJSON_AddItemToArray(arr, n3);

    // step 4: Operate
    sz_before = cJSON_GetArraySize(arr);
    cJSON_DeleteItemFromArray(arr, 1);
    sz_after = cJSON_GetArraySize(arr);

    // step 5: Validate
    validation_score = (int)added_arr + (int)added1 + (int)added2 + (int)added3 + (sz_before == 3) + (sz_after == 2);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    // API sequence test completed successfully
    return 66;
}
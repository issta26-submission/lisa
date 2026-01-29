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
//<ID> 1334
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
    cJSON *n0 = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *flag = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    cJSON_bool flag_is_false = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(10.0);
    n1 = cJSON_CreateNumber(20.0);
    flag = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_InsertItemInArray(arr, 0, n0);
    cJSON_InsertItemInArray(arr, 1, n1);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToObject(root, "flag", flag);

    // step 4: Operate
    retrieved_flag = cJSON_GetObjectItem(root, "flag");
    flag_is_false = cJSON_IsFalse(retrieved_flag);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(n0 != (cJSON *)0) + (int)(n1 != (cJSON *)0) + (int)(flag_is_false != 0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
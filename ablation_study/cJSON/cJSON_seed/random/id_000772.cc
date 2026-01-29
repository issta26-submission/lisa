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
//<ID> 772
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
    cJSON *false_item0 = (cJSON *)0;
    cJSON *false_item1 = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool is_false0 = 0;
    cJSON_bool is_false1 = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure
    cJSON_AddItemToObject(root, "items", arr);
    false_item0 = cJSON_CreateFalse();
    false_item1 = cJSON_CreateFalse();
    added0 = cJSON_AddItemToArray(arr, false_item0);
    added1 = cJSON_AddItemToArray(arr, false_item1);

    // step 4: Operate
    retrieved = cJSON_GetArrayItem(arr, 0);
    is_false0 = cJSON_IsFalse(retrieved);
    retrieved = cJSON_GetArrayItem(arr, 1);
    is_false1 = cJSON_IsFalse(retrieved);

    // step 5: Validate
    validation_score = (int)added0 + (int)added1 + (int)is_false0 + (int)is_false1 + (arr != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
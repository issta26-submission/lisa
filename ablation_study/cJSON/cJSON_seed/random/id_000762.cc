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
//<ID> 762
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
    cJSON *first_item = (cJSON *)0;
    cJSON *second_item = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON *copied_item = (cJSON *)0;
    char *retrieved_str = (char *)0;
    cJSON_bool added_array = 0;
    cJSON_bool added_copied = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    first_item = cJSON_CreateString("alpha");
    second_item = cJSON_CreateString("beta");

    // step 3: Configure
    cJSON_AddItemToArray(arr, first_item);
    cJSON_AddItemToArray(arr, second_item);
    added_array = cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    retrieved = cJSON_GetArrayItem(arr, 0);
    retrieved_str = cJSON_GetStringValue(retrieved);
    copied_item = cJSON_CreateString(retrieved_str ? retrieved_str : "");
    added_copied = cJSON_AddItemToObject(root, "copied_first", copied_item);

    // step 5: Validate
    validation_score = (int)added_array + (int)added_copied + (int)cJSON_IsObject(root) + (retrieved != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
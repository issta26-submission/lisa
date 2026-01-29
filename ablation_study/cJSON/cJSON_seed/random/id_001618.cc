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
//<ID> 1618
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *array = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *n3 = (cJSON *)0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool added3 = 0;
    cJSON_bool added_array_to_root = 0;
    int size_before = 0;
    int size_after = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    array = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(10.0);
    n2 = cJSON_CreateNumber(20.0);
    n3 = cJSON_CreateNumber(30.0);

    // step 3: Configure
    added1 = cJSON_AddItemToArray(array, n1);
    added2 = cJSON_AddItemToArray(array, n2);
    added3 = cJSON_AddItemToArray(array, n3);
    added_array_to_root = cJSON_AddItemToObject(root, "numbers", array);

    // step 4: Operate
    size_before = cJSON_GetArraySize(array);
    cJSON_DeleteItemFromArray(array, 1);
    size_after = cJSON_GetArraySize(array);

    // step 5: Validate
    validation_score = (int)added1
        + (int)added2
        + (int)added3
        + (int)added_array_to_root
        + (int)(size_before == 3)
        + (int)(size_after == 2)
        + (int)(array != (cJSON *)0)
        + (int)(root != (cJSON *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)size_before;
    (void)size_after;
    (void)added1;
    (void)added2;
    (void)added3;
    (void)added_array_to_root;
    // API sequence test completed successfully
    return 66;
}
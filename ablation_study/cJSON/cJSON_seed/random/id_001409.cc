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
//<ID> 1409
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
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *flag_false = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON *elem0_ref = (cJSON *)0;
    cJSON *found_elem = (cJSON *)0;
    int arr_size = 0;
    int is_false = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array, two string elements and a false boolean
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    elem0 = cJSON_CreateString("first");
    elem1 = cJSON_CreateString("second");
    flag_false = cJSON_CreateFalse();

    // step 3: Configure - populate array and attach members to root using CS add (constant string)
    cJSON_AddItemToArray(arr, elem0);
    cJSON_AddItemToArray(arr, elem1);
    cJSON_AddItemToObjectCS(root, "items", arr);
    cJSON_AddItemToObjectCS(root, "flag", flag_false);
    arr_ref = cJSON_CreateObjectReference(arr);
    cJSON_AddItemToObjectCS(root, "items_ref", arr_ref);

    // step 4: Operate - retrieve first array element, create a reference to it and attach to root
    found_elem = cJSON_GetArrayItem(arr, 0);
    elem0_ref = cJSON_CreateObjectReference(found_elem);
    cJSON_AddItemToObjectCS(root, "first_ref", elem0_ref);

    // step 5: Validate - check array size and boolean, derive a small validation score
    arr_size = cJSON_GetArraySize(arr);
    is_false = (int)cJSON_IsFalse(flag_false);
    validation_score = arr_size + is_false + (int)(arr_ref != (cJSON *)0) + (int)(elem0_ref != (cJSON *)0);

    // step 6: Cleanup - delete the whole tree (references are part of the tree)
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)found_elem;
    (void)arr_size;
    (void)is_false;
    return 66;
}
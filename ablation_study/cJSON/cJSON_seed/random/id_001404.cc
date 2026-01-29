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
//<ID> 1404
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
    cJSON *false_node = (cJSON *)0;
    cJSON *ref_to_elem0 = (cJSON *)0;
    cJSON *got_elem = (cJSON *)0;
    int arr_size = 0;
    int false_is_false = 0;
    int validation_score = 0;
    cJSON_bool add_res1 = 0;
    cJSON_bool add_res2 = 0;

    // step 2: Initialize - create root object, array, two elements and a false boolean
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    elem0 = cJSON_CreateString("first");
    elem1 = cJSON_CreateString("second");
    false_node = cJSON_CreateFalse();

    // step 3: Configure - populate array and attach array and flag to root using CS add
    cJSON_AddItemToArray(arr, elem0);
    cJSON_AddItemToArray(arr, elem1);
    add_res1 = cJSON_AddItemToObjectCS(root, "items", arr);
    add_res2 = cJSON_AddItemToObjectCS(root, "flag", false_node);

    // step 4: Operate - create an object reference to the first element and add it to root, retrieve first element via array API
    ref_to_elem0 = cJSON_CreateObjectReference(elem0);
    cJSON_AddItemToObjectCS(root, "first_ref", ref_to_elem0);
    got_elem = cJSON_GetArrayItem(arr, 0);

    // step 5: Validate - collect array size and boolean state and compute a simple validation score
    arr_size = cJSON_GetArraySize(arr);
    false_is_false = (int)cJSON_IsFalse(false_node);
    validation_score = arr_size + false_is_false + (int)(got_elem != (cJSON *)0) + (int)(ref_to_elem0 != (cJSON *)0) + (int)(add_res1 != 0) + (int)(add_res2 != 0);

    // step 6: Cleanup - delete the root which contains the whole tree
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)arr_size;
    (void)false_is_false;
    (void)got_elem;
    (void)ref_to_elem0;
    (void)add_res1;
    (void)add_res2;
    return 66;
}
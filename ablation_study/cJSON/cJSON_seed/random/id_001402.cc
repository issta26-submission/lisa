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
//<ID> 1402
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
    cJSON *ref_to_false = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    int arr_size = 0;
    int flag_is_false = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array, two string elements and a false boolean
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    elem0 = cJSON_CreateString("first");
    elem1 = cJSON_CreateString("second");
    false_node = cJSON_CreateFalse();

    // step 3: Configure - populate the array and attach members to root using case-sensitive object add
    cJSON_AddItemToArray(arr, elem0);
    cJSON_AddItemToArray(arr, elem1);
    cJSON_AddItemToObjectCS(root, "numbers", arr);
    cJSON_AddItemToObjectCS(root, "flag", false_node);

    // step 4: Operate - create an object reference to the false node, add it to root, and get the first array item
    ref_to_false = cJSON_CreateObjectReference(false_node);
    cJSON_AddItemToObjectCS(root, "flag_ref", ref_to_false);
    first_elem = cJSON_GetArrayItem(arr, 0);

    // step 5: Validate - compute array size, check boolean node and a simple validation score
    arr_size = cJSON_GetArraySize(arr);
    flag_is_false = (int)cJSON_IsFalse(false_node);
    validation_score = arr_size + flag_is_false + (int)(first_elem != (cJSON *)0) + (int)(ref_to_false != (cJSON *)0);

    // step 6: Cleanup - delete the entire tree to free resources
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)arr_size;
    (void)flag_is_false;
    (void)first_elem;
    return 66;
}
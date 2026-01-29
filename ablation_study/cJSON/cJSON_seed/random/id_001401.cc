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
//<ID> 1401
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
    cJSON *item1 = (cJSON *)0;
    cJSON *item2 = (cJSON *)0;
    cJSON *false_node = (cJSON *)0;
    cJSON *false_ref = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    int arr_size = 0;
    int flag_is_false = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item1 = cJSON_CreateString("elem_one");
    item2 = cJSON_CreateString("elem_two");
    false_node = cJSON_CreateFalse();

    // step 3: Configure - populate array and attach members to root using case-sensitive add
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, item2);
    cJSON_AddItemToObjectCS(root, "list", arr);
    false_ref = cJSON_CreateObjectReference(false_node);
    cJSON_AddItemToObjectCS(root, "flag", false_ref);

    // step 4: Operate - retrieve first element from array
    first_elem = cJSON_GetArrayItem(arr, 0);

    // step 5: Validate - compute simple validation metrics
    arr_size = cJSON_GetArraySize(arr);
    flag_is_false = (int)cJSON_IsFalse(cJSON_GetObjectItem(root, "flag"));
    validation_score = arr_size + (int)(first_elem != (cJSON *)0) + flag_is_false + (int)(false_ref != (cJSON *)0);

    // step 6: Cleanup - delete main tree then the separately created false node reference target
    cJSON_Delete(root);
    cJSON_Delete(false_node);

    // API sequence test completed successfully
    (void)validation_score;
    (void)arr_size;
    (void)flag_is_false;
    (void)first_elem;
    return 66;
}
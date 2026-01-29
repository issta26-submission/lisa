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
//<ID> 1403
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
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *flag_false = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *first_ref = (cJSON *)0;
    int arr_size = 0;
    int flag_is_false = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and elements including a false boolean
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item0 = cJSON_CreateString("zero");
    item1 = cJSON_CreateString("one");
    flag_false = cJSON_CreateFalse();

    // step 3: Configure - populate array and attach members to root using case-sensitive add
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToObjectCS(root, "data", arr);
    cJSON_AddItemToObjectCS(root, "flag", flag_false);

    // step 4: Operate - retrieve the first array element, create an object reference to it and attach
    first_elem = cJSON_GetArrayItem(arr, 0);
    first_ref = cJSON_CreateObjectReference(first_elem);
    cJSON_AddItemToObjectCS(root, "first_ref", first_ref);

    // step 5: Validate - compute simple validations from the constructed tree
    arr_size = cJSON_GetArraySize(arr);
    flag_is_false = (int)cJSON_IsFalse(flag_false);
    validation_score = arr_size + flag_is_false + (int)(first_ref != (cJSON *)0);

    // step 6: Cleanup - delete the entire tree
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)arr_size;
    (void)flag_is_false;
    (void)first_elem;
    return 66;
}
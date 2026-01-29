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
//<ID> 1400
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
    cJSON *str0 = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *flag_false = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *first_ref = (cJSON *)0;
    cJSON *flag_ref = (cJSON *)0;
    int arr_size = 0;
    int flag_false_flag = 0;
    int validation_score = 0;

    // step 2: Initialize - create root, an array, two strings and a false boolean
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str0 = cJSON_CreateString("alpha");
    str1 = cJSON_CreateString("beta");
    flag_false = cJSON_CreateFalse();

    // step 3: Configure - populate the array and attach members to root (case-sensitive keys)
    cJSON_AddItemToArray(arr, str0);
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToObjectCS(root, "data", arr);
    cJSON_AddItemToObjectCS(root, "flag", flag_false);

    // step 4: Operate - obtain the first array element, create object references and attach them
    first_elem = cJSON_GetArrayItem(arr, 0);
    first_ref = cJSON_CreateObjectReference(first_elem);
    flag_ref = cJSON_CreateObjectReference(flag_false);
    cJSON_AddItemToObjectCS(root, "first_ref", first_ref);
    cJSON_AddItemToObjectCS(root, "flag_ref", flag_ref);

    // step 5: Validate - compute array size and boolean check and a simple validation score
    arr_size = cJSON_GetArraySize(arr);
    flag_false_flag = (int)cJSON_IsFalse(flag_false);
    validation_score = arr_size + flag_false_flag + (int)(first_ref != (cJSON *)0) + (int)(flag_ref != (cJSON *)0);

    // step 6: Cleanup - delete the entire tree (references are children of root)
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)arr_size;
    (void)flag_false_flag;
    (void)first_elem;
    return 66;
}
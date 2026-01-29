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
//<ID> 1389
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
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON *flag_false = (cJSON *)0;
    cJSON *found_arr = (cJSON *)0;
    cJSON *found_flag = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *first_copy = (cJSON *)0;
    int arr_size = 0;
    int flag_is_false = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("one");
    s2 = cJSON_CreateString("two");
    flag_false = cJSON_CreateFalse();

    // step 3: Configure - populate array and attach members to root
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToObject(root, "flag", flag_false);

    // step 4: Operate - retrieve members, duplicate first array item and add it to root
    found_arr = cJSON_GetObjectItem(root, "data");
    found_flag = cJSON_GetObjectItem(root, "flag");
    first_elem = cJSON_GetArrayItem(found_arr, 0);
    first_copy = cJSON_Duplicate(first_elem, 1);
    cJSON_AddItemToObject(root, "first_copy", first_copy);

    // step 5: Validate - gather array size and boolean check, compute a simple score
    arr_size = cJSON_GetArraySize(found_arr);
    flag_is_false = (int)cJSON_IsFalse(found_flag);
    validation_score = (int)(root != (cJSON *)0) + arr_size + flag_is_false + (int)(first_copy != (cJSON *)0);

    // step 6: Cleanup - free all cJSON resources
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)arr_size;
    (void)flag_is_false;
    (void)found_arr;
    (void)found_flag;
    (void)first_elem;
    return 66;
}
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
//<ID> 1407
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
    cJSON *arr_ref = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    int arr_size = 0;
    int is_false = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item0 = cJSON_CreateString("zero");
    item1 = cJSON_CreateString("one");
    flag_false = cJSON_CreateFalse();

    // step 3: Configure - populate array and attach members to root (case-sensitive keys)
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToObjectCS(root, "arr", arr);
    cJSON_AddItemToObjectCS(root, "flag", flag_false);

    // step 4: Operate - create an object reference to the array and fetch the first element
    arr_ref = cJSON_CreateObjectReference((const cJSON *)arr);
    cJSON_AddItemToObjectCS(root, "arr_ref", arr_ref);
    first_elem = cJSON_GetArrayItem(arr, 0);

    // step 5: Validate - compute simple metrics from the structure
    arr_size = cJSON_GetArraySize(arr);
    is_false = (int)cJSON_IsFalse(cJSON_GetObjectItem(root, "flag"));
    validation_score = arr_size + (int)(first_elem != (cJSON *)0) + is_false;

    // step 6: Cleanup - free all cJSON resources
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)arr_size;
    (void)is_false;
    (void)first_elem;
    (void)item0;
    (void)item1;
    (void)arr_ref;
    return 66;
}
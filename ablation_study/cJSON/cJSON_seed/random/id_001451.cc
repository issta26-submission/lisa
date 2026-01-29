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
//<ID> 1451
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
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *replacement = (cJSON *)0;
    cJSON *picked_arr = (cJSON *)0;
    cJSON *picked_item = (cJSON *)0;
    int validation_score = 0;
    cJSON_bool added = 0;
    cJSON_bool replaced = 0;

    // step 2: Initialize - create root object, an array, and two numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(7.0);
    num1 = cJSON_CreateNumber(14.0);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);

    // step 3: Configure - attach the array to the root object under the key "items"
    added = cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate - retrieve the array via object lookup, pick the second element, and replace it
    picked_arr = cJSON_GetObjectItem(root, "items");
    picked_item = cJSON_GetArrayItem(picked_arr, 1);
    replacement = cJSON_CreateNumber(42.0);
    replaced = cJSON_ReplaceItemViaPointer(picked_arr, picked_item, replacement);

    // step 5: Validate - derive a small validation score from results
    validation_score = (int)(added != 0) + (int)(replaced != 0) + (int)(cJSON_GetArrayItem(arr, 1) == replacement);

    // step 6: Cleanup - delete the whole JSON tree and silence unused-variable warnings
    cJSON_Delete(root);
    (void)validation_score;
    (void)num0;
    (void)num1;
    (void)picked_item;
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1330
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
    cJSON *itm_false = (cJSON *)0;
    cJSON *itm_true = (cJSON *)0;
    cJSON *itm_num = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON_bool inserted0 = 0;
    cJSON_bool inserted1 = 0;
    cJSON_bool inserted2 = 0;
    cJSON_bool added = 0;
    cJSON_bool is_false = 0;
    double num_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and three items (false, true, number)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    itm_false = cJSON_CreateFalse();
    itm_true = cJSON_CreateTrue();
    itm_num = cJSON_CreateNumber(42.0);

    // step 3: Configure - insert items into the array at specific positions and attach to root
    inserted0 = cJSON_InsertItemInArray(arr, 0, itm_false);
    inserted1 = cJSON_InsertItemInArray(arr, 1, itm_true);
    inserted2 = cJSON_InsertItemInArray(arr, 1, itm_num); // inserts number between false and true
    added = cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate - retrieve the array and examine elements (check false and the numeric value)
    retrieved = cJSON_GetObjectItem(root, "items");
    elem0 = cJSON_GetArrayItem(retrieved, 0);
    elem1 = cJSON_GetArrayItem(retrieved, 1);
    is_false = cJSON_IsFalse(elem0);
    num_value = cJSON_GetNumberValue(elem1);

    // step 5: Validate - compute a simple validation score from outcomes
    validation_score = (int)(root != (cJSON *)0) + (int)(retrieved != (cJSON *)0) + (int)inserted0 + (int)inserted1 + (int)inserted2 + (int)added + (int)is_false + (int)(num_value == 42.0);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
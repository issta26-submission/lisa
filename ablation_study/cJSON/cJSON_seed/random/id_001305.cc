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
//<ID> 1305
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *items = (cJSON *)0;
    cJSON *flag = (cJSON *)0;
    cJSON *note_item = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    cJSON *retrieved_items = (cJSON *)0;
    cJSON *retrieved_note = (cJSON *)0;
    int items_count = 0;
    cJSON_bool flag_is_false = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize - create the root object, an array and a false boolean
    root = cJSON_CreateObject();
    items = cJSON_CreateArray();
    flag = cJSON_CreateFalse();

    // step 3: Configure - attach the array and boolean to the root and add a string via helper API
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddItemToObject(root, "items", items);
    note_item = cJSON_AddStringToObject(root, "note", "example_note");

    // step 4: Operate - populate the array and retrieve items case-sensitively, inspect values
    cJSON_AddItemToArray(items, cJSON_CreateString("one"));
    cJSON_AddItemToArray(items, cJSON_CreateString("two"));
    cJSON_AddItemToArray(items, cJSON_CreateString("three"));
    retrieved_flag = cJSON_GetObjectItemCaseSensitive(root, "flag");
    flag_is_false = cJSON_IsFalse(retrieved_flag);
    retrieved_items = cJSON_GetObjectItemCaseSensitive(root, "items");
    items_count = cJSON_GetArraySize(retrieved_items);
    retrieved_note = cJSON_GetObjectItemCaseSensitive(root, "note");

    // step 5: Validate - compute a small validation score from observed state
    validation_score = (int)(root != (cJSON *)0) + (int)(items != (cJSON *)0) + (int)flag_is_false + (int)(items_count == 3) + (int)(retrieved_note != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees all attached children
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
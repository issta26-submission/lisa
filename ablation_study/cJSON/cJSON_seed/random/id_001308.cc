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
//<ID> 1308
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *array = (cJSON *)0;
    cJSON *elem_str = (cJSON *)0;
    cJSON *elem_false = (cJSON *)0;
    cJSON *retrieved_items = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    cJSON *retrieved_name = (cJSON *)0;
    const char *name_str = (const char *)0;
    int array_size = 0;
    cJSON_bool first_is_false = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize - create root object and components
    root = cJSON_CreateObject();
    array = cJSON_CreateArray();
    elem_str = cJSON_CreateString("element1");
    elem_false = cJSON_CreateFalse();

    // step 3: Configure - populate array and attach to root, add a named string
    cJSON_AddItemToArray(array, elem_str);
    cJSON_AddItemToArray(array, elem_false);
    cJSON_AddItemToObject(root, "items", array);
    cJSON_AddStringToObject(root, "name", "tester");

    // step 4: Operate - retrieve items and inspect contents
    retrieved_items = cJSON_GetObjectItemCaseSensitive(root, "items");
    array_size = cJSON_GetArraySize(retrieved_items);
    first_item = cJSON_GetArrayItem(retrieved_items, 0);
    first_is_false = cJSON_IsFalse(first_item);
    retrieved_name = cJSON_GetObjectItemCaseSensitive(root, "name");
    name_str = cJSON_GetStringValue(retrieved_name);

    // step 5: Validate - compute a simple validation score from results
    validation_score = (int)(root != (cJSON *)0) + array_size + (int)first_is_false + (int)(name_str != (const char *)0);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
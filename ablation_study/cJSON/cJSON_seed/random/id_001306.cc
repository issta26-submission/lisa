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
//<ID> 1306
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
    cJSON *elem1 = (cJSON *)0;
    cJSON *elem2 = (cJSON *)0;
    cJSON *flag = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    cJSON *retrieved_items = (cJSON *)0;
    const char *name_str = (const char *)0;
    cJSON_bool flag_is_false = (cJSON_bool)0;
    int items_count = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, array and element nodes
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    elem1 = cJSON_CreateString("one");
    elem2 = cJSON_CreateString("two");
    flag = cJSON_CreateFalse();

    // step 3: Configure - assemble array and attach to root, add flag and name
    cJSON_AddItemToArray(arr, elem1);
    cJSON_AddItemToArray(arr, elem2);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "flag", flag);
    name_item = cJSON_AddStringToObject(root, "name", "example");

    // step 4: Operate - retrieve items and inspect types/values
    retrieved_flag = cJSON_GetObjectItemCaseSensitive(root, "flag");
    flag_is_false = cJSON_IsFalse(retrieved_flag);
    retrieved_items = cJSON_GetObjectItemCaseSensitive(root, "items");
    items_count = cJSON_GetArraySize(retrieved_items);
    name_str = cJSON_GetStringValue(name_item);

    // step 5: Validate - compute a simple validation score from results
    validation_score = (int)flag_is_false + items_count + (int)(name_str != (const char *)0);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
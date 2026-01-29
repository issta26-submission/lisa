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
//<ID> 1133
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *items_array = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *fetched_name = (cJSON *)0;
    cJSON *fetched_items = (cJSON *)0;
    cJSON *fetched_elem0 = (cJSON *)0;
    char *name_value = (char *)0;
    char *elem0_value = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object and members
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name_item);
    items_array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items_array);

    // step 3: Configure - populate the array with string elements
    elem0 = cJSON_CreateString("first");
    elem1 = cJSON_CreateString("second");
    cJSON_AddItemToArray(items_array, elem0);
    cJSON_AddItemToArray(items_array, elem1);

    // step 4: Operate - retrieve object members and array elements
    fetched_name = cJSON_GetObjectItem(root, "name");
    name_value = cJSON_GetStringValue(fetched_name);
    fetched_items = cJSON_GetObjectItem(root, "items");
    fetched_elem0 = cJSON_GetArrayItem(fetched_items, 0);
    elem0_value = cJSON_GetStringValue(fetched_elem0);

    // step 5: Validate - perform simple validations (no branches)
    validation_score = (int)(root != (cJSON *)0) + (int)(fetched_name != (cJSON *)0) + (int)(name_value != (char *)0) + (int)(fetched_items != (cJSON *)0) + (int)(fetched_elem0 != (cJSON *)0) + (int)(name_value[0] == 'e') + (int)(elem0_value[0] == 'f');
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
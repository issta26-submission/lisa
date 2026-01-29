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
//<ID> 484
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
    cJSON *name_item = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    char *printed = (char *)0;
    const char *name_val = (const char *)0;
    cJSON_bool is_items_array = 0;
    cJSON_bool first_is_true = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    items = cJSON_CreateArray();

    // step 3: Configure
    name_item = cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddFalseToObject(root, "flag_false");
    cJSON_AddBoolToObject(root, "flag_explicit", 1);
    cJSON_AddItemToArray(items, cJSON_CreateTrue());

    // step 4: Operate
    is_items_array = cJSON_IsArray(items);
    first_item = cJSON_GetArrayItem(items, 0);
    first_is_true = cJSON_IsTrue(first_item);
    name_val = cJSON_GetStringValue(name_item);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    (void)is_items_array;
    (void)first_is_true;
    (void)name_val;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
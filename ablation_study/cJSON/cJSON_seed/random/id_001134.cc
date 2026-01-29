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
//<ID> 1134
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
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *fetched_name = (cJSON *)0;
    cJSON *fetched_items = (cJSON *)0;
    cJSON *fetched_elem0 = (cJSON *)0;
    cJSON *fetched_elem1 = (cJSON *)0;
    char *name_str = (char *)0;
    char *elem0_str = (char *)0;
    char *elem1_str = (char *)0;
    char *printed = (char *)0;
    int validation_score = 0;
    cJSON_bool ok_name = (cJSON_bool)0;
    cJSON_bool ok_elem0 = (cJSON_bool)0;
    cJSON_bool ok_elem1 = (cJSON_bool)0;

    // step 2: Initialize - create root object and basic items
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("device42");
    items = cJSON_CreateArray();

    // step 3: Configure - attach name and populate array with string elements
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddItemToObject(root, "items", items);
    elem0 = cJSON_CreateString("alpha");
    elem1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, elem0);
    cJSON_AddItemToArray(items, elem1);

    // step 4: Operate - retrieve object members and array elements, serialize
    fetched_name = cJSON_GetObjectItem(root, "name");
    name_str = cJSON_GetStringValue(fetched_name);
    fetched_items = cJSON_GetObjectItem(root, "items");
    fetched_elem0 = cJSON_GetArrayItem(fetched_items, 0);
    fetched_elem1 = cJSON_GetArrayItem(fetched_items, 1);
    elem0_str = cJSON_GetStringValue(fetched_elem0);
    elem1_str = cJSON_GetStringValue(fetched_elem1);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate - check types and presence of string values, free printed buffer
    ok_name = cJSON_IsString(fetched_name);
    ok_elem0 = cJSON_IsString(fetched_elem0);
    ok_elem1 = cJSON_IsString(fetched_elem1);
    validation_score = (int)ok_name + (int)ok_elem0 + (int)ok_elem1 + (int)(name_str != (char *)0) + (int)(elem0_str != (char *)0) + (int)(elem1_str != (char *)0);
    cJSON_free((void *)printed);
    (void)validation_score;

    // step 6: Cleanup - delete root (frees all attached items)
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
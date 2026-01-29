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
//<ID> 1139
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *info = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *list = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *fetched_info = (cJSON *)0;
    cJSON *fetched_name = (cJSON *)0;
    cJSON *fetched_list = (cJSON *)0;
    cJSON *fetched_elem1 = (cJSON *)0;
    char *printed = (char *)0;
    char *name_str = (char *)0;
    char *elem1_str = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object and sub-objects/items
    root = cJSON_CreateObject();
    info = cJSON_CreateObject();
    name_item = cJSON_CreateString("demo-name");
    cJSON_AddItemToObject(info, "name", name_item);
    cJSON_AddItemToObject(root, "info", info);

    // step 3: Configure - create an array and populate it with strings, attach to root
    list = cJSON_CreateArray();
    elem0 = cJSON_CreateString("first-element");
    elem1 = cJSON_CreateString("second-element");
    cJSON_AddItemToArray(list, elem0);
    cJSON_AddItemToArray(list, elem1);
    cJSON_AddItemToObject(root, "list", list);

    // step 4: Operate - retrieve nested object members and array elements
    fetched_info = cJSON_GetObjectItem(root, "info");
    fetched_name = cJSON_GetObjectItem(fetched_info, "name");
    name_str = cJSON_GetStringValue(fetched_name);
    fetched_list = cJSON_GetObjectItem(root, "list");
    fetched_elem1 = cJSON_GetArrayItem(fetched_list, 1);
    elem1_str = cJSON_GetStringValue(fetched_elem1);

    // step 5: Validate - serialize for round-trip usage and compute a simple validation score
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);
    validation_score = (int)(root != (cJSON *)0) + (int)(name_str != (char *)0) + (int)(elem1_str != (char *)0);
    (void)validation_score;

    // step 6: Cleanup - delete root which also frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
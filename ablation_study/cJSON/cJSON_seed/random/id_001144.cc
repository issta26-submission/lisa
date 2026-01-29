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
//<ID> 1144
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *value_num = (cJSON *)0;
    cJSON *items = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *fetched_meta = (cJSON *)0;
    cJSON *fetched_value = (cJSON *)0;
    cJSON *fetched_items = (cJSON *)0;
    cJSON *fetched_elem0 = (cJSON *)0;
    cJSON *detached_item = (cJSON *)0;
    char *detached_str = (char *)0;
    double numeric_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object and sub-objects
    root = cJSON_CreateObject();
    meta = cJSON_CreateObject();
    value_num = cJSON_CreateNumber(2025.0);
    cJSON_AddItemToObject(meta, "value", value_num);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Configure - create an array with a string and a number, attach to root
    items = cJSON_CreateArray();
    elem0 = cJSON_CreateString("detachable-string");
    elem1 = cJSON_CreateNumber(7.5);
    cJSON_AddItemToArray(items, elem0);
    cJSON_AddItemToArray(items, elem1);
    cJSON_AddItemToObject(root, "items", items);

    // step 4: Operate - retrieve object member and array element, detach element by pointer
    fetched_meta = cJSON_GetObjectItem(root, "meta");
    fetched_value = cJSON_GetObjectItem(fetched_meta, "value");
    numeric_value = cJSON_GetNumberValue(fetched_value);
    fetched_items = cJSON_GetObjectItem(root, "items");
    fetched_elem0 = cJSON_GetArrayItem(fetched_items, 0);
    detached_item = cJSON_DetachItemViaPointer(fetched_items, fetched_elem0);
    detached_str = cJSON_GetStringValue(detached_item);

    // step 5: Validate - compute a small validation score based on retrieved values
    validation_score = (int)(root != (cJSON *)0) + (int)(fetched_meta != (cJSON *)0) + (int)(detached_item != (cJSON *)0) + (int)(detached_str != (char *)0) + (int)(numeric_value == 2025.0);
    (void)validation_score;

    // step 6: Cleanup - delete root and the detached item
    cJSON_Delete(root);
    cJSON_Delete(detached_item);

    // API sequence test completed successfully
    return 66;
}
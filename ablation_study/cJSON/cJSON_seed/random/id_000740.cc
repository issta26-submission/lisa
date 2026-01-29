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
//<ID> 740
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
    cJSON *false_item1 = (cJSON *)0;
    cJSON *false_item2 = (cJSON *)0;
    cJSON *retrieved_items = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    int items_count = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    items = cJSON_CreateArray();
    false_item1 = cJSON_CreateFalse();
    false_item2 = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToArray(items, false_item1);
    cJSON_AddItemToArray(items, false_item2);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 4: Operate
    retrieved_items = cJSON_GetObjectItem(root, "items");
    items_count = cJSON_GetArraySize(retrieved_items);
    retrieved_flag = cJSON_GetObjectItem(root, "enabled");

    // step 5: Validate
    validation_score = items_count + (retrieved_flag != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
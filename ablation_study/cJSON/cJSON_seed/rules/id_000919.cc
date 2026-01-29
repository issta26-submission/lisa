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
//<ID> 919
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *items = NULL;
    cJSON *name = NULL;
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    cJSON *retrieved_name = NULL;
    cJSON *retrieved_items = NULL;
    const char *initial_name = "example_name";
    const char *val0 = "one";
    const char *val1 = "two";
    const char *val2 = "three";
    const char *retrieved_name_str = NULL;
    int count_before = 0;
    int count_after = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name = cJSON_CreateStringReference(initial_name);
    cJSON_AddItemToObject(root, "name", name);
    items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    item0 = cJSON_CreateStringReference(val0);
    cJSON_AddItemToArray(items, item0);
    item1 = cJSON_CreateStringReference(val1);
    cJSON_AddItemToArray(items, item1);
    item2 = cJSON_CreateStringReference(val2);
    cJSON_AddItemToArray(items, item2);

    // step 3: Operate / Validate
    retrieved_name = cJSON_GetObjectItemCaseSensitive(root, "name");
    retrieved_name_str = cJSON_GetStringValue(retrieved_name);
    retrieved_items = cJSON_GetObjectItemCaseSensitive(root, "items");
    count_before = cJSON_GetArraySize(retrieved_items);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "name");
    count_after = cJSON_GetArraySize(cJSON_GetObjectItemCaseSensitive(root, "items"));

    // step 4: Cleanup
    (void)retrieved_name_str;
    (void)count_before;
    (void)count_after;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 918
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
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *retrieved_items = NULL;
    char *printed = NULL;
    int size_before = 0;
    int size_after = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    item0 = cJSON_CreateStringReference("alpha");
    cJSON_AddItemToArray(items, item0);
    item1 = cJSON_CreateStringReference("beta");
    cJSON_AddItemToArray(items, item1);
    cJSON_AddItemToObject(root, "label", cJSON_CreateStringReference("mylabel"));

    // step 3: Operate / Validate
    retrieved_items = cJSON_GetObjectItemCaseSensitive(root, "items");
    size_before = cJSON_GetArraySize(retrieved_items);
    printed = cJSON_PrintUnformatted(root);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "label");
    size_after = cJSON_GetArraySize(retrieved_items);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)size_before;
    (void)size_after;
    return 66;
}
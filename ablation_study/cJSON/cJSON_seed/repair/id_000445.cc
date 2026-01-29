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
//<ID> 445
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *it0 = cJSON_CreateString("alpha");
    cJSON *it1 = cJSON_CreateString("beta");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, it0);
    cJSON_AddItemToArray(items, it1);
    cJSON_AddStringToObject(meta, "title", "DocumentTitle");

    // step 2: Configure
    cJSON_AddStringToObject(root, "status", "active");
    cJSON_AddStringToObject(meta, "author", "bob@example.com");
    cJSON *it2 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(items, it2);

    // step 3: Operate
    cJSON *meta_from_root = cJSON_GetObjectItem(root, "meta");
    cJSON *title_item = cJSON_GetObjectItem(meta_from_root, "title");
    cJSON_bool title_is_string = cJSON_IsString(title_item);
    char *title_text = cJSON_GetStringValue(title_item);
    cJSON *items_from_root = cJSON_GetObjectItem(root, "items");
    cJSON_DeleteItemFromArray(items_from_root, 1);

    // step 4: Validate and Cleanup
    (void)title_is_string;
    (void)title_text;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
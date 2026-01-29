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
//<ID> 446
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);
    cJSON_AddStringToObject(info, "title", "ExampleDocument");
    cJSON_AddStringToObject(info, "author", "tester@example.com");
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(items, cJSON_CreateString("beta"));
    cJSON_AddItemToArray(items, cJSON_CreateString("gamma"));

    // step 2: Configure
    cJSON_AddStringToObject(root, "status", "active");
    cJSON *info_from_root = cJSON_GetObjectItem(root, "info");
    cJSON *title_item = cJSON_GetObjectItem(info_from_root, "title");

    // step 3: Operate
    cJSON_bool title_is_string = cJSON_IsString(title_item);
    cJSON_DeleteItemFromArray(items, 1);
    int items_count = cJSON_GetArraySize(items);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON *last_item = cJSON_GetArrayItem(items, items_count - 1);
    char *snapshot = cJSON_Print(root);

    // step 4: Validate and Cleanup
    const char *first_val = cJSON_GetStringValue(first_item);
    const char *last_val = cJSON_GetStringValue(last_item);
    (void)title_is_string;
    (void)first_val;
    (void)last_val;
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
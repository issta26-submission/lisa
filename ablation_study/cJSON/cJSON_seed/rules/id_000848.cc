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
//<ID> 848
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *name_item = NULL;
    cJSON *present_item = NULL;
    cJSON *raw_item = NULL;
    cJSON *fetched_raw = NULL;
    char *printed = NULL;
    cJSON_bool is_raw = 0;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(root, "name", name_item);
    present_item = cJSON_AddTrueToObject(root, "present");
    raw_item = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToObject(root, "meta_raw", raw_item);

    // step 3: Operate / Validate
    fetched_raw = cJSON_GetObjectItemCaseSensitive(root, "meta_raw");
    is_raw = cJSON_IsRaw(fetched_raw);
    printed = cJSON_PrintUnformatted(root);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "name");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    (void)is_raw;
    (void)present_item;
    (void)raw_item;
    (void)fetched_raw;
    return 66;
}
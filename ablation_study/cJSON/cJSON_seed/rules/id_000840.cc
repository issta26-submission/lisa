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
//<ID> 840
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
    cJSON *true_item = NULL;
    cJSON *raw_item = NULL;
    cJSON *dup_root = NULL;
    char *printed = NULL;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool prealloc_ok = 0;
    cJSON_bool raw_flag = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(root, "name", name_item);
    true_item = cJSON_AddTrueToObject(root, "enabled");
    raw_item = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToObject(root, "meta_raw", raw_item);

    // step 3: Operate / Validate
    raw_item = cJSON_GetObjectItemCaseSensitive(root, "meta_raw");
    raw_flag = cJSON_IsRaw(raw_item);
    dup_root = cJSON_Duplicate(root, 1);
    printed = cJSON_PrintUnformatted(dup_root);
    prealloc_ok = cJSON_PrintPreallocated(dup_root, buffer, (int)sizeof(buffer), 1);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "name");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));
    (void)raw_flag;
    (void)prealloc_ok;
    (void)true_item;

    // API sequence test completed successfully
    return 66;
}
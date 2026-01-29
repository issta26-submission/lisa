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
//<ID> 845
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
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool raw_is_raw = 0;
    cJSON_bool equal_dup = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(root, "name", name_item);
    true_item = cJSON_AddTrueToObject(root, "active");
    raw_item = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToObject(root, "meta_raw", raw_item);

    // step 3: Operate / Validate
    raw_is_raw = cJSON_IsRaw(raw_item);
    printed = cJSON_PrintUnformatted(root);
    dup_root = cJSON_Duplicate(root, 1);
    equal_dup = cJSON_Compare(root, dup_root, 1);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "name");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    (void)raw_is_raw;
    (void)true_item;
    (void)equal_dup;
    return 66;
}
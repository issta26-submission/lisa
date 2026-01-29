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
//<ID> 843
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
    cJSON *raw_item = NULL;
    cJSON *retrieved_raw = NULL;
    cJSON *dup_root = NULL;
    char *printed = NULL;
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool is_raw = 0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name_item);
    raw_item = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToObject(root, "meta", raw_item);
    cJSON_AddTrueToObject(root, "enabled");

    // step 3: Operate / Validate
    retrieved_raw = cJSON_GetObjectItemCaseSensitive(root, "meta");
    is_raw = cJSON_IsRaw(retrieved_raw);
    dup_root = cJSON_Duplicate(root, 1);
    printed = cJSON_PrintUnformatted(dup_root);
    equal = cJSON_Compare(root, dup_root, 1);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "name");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    (void)is_raw;
    (void)equal;
    // API sequence test completed successfully
    return 66;
}
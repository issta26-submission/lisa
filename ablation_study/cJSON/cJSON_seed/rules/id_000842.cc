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
//<ID> 842
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
    cJSON *dup_root = NULL;
    char *printed = NULL;
    char *printed_dup = NULL;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool prealloc_ok = 0;
    cJSON_bool raw_check = 0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddTrueToObject(root, "enabled");
    raw_item = cJSON_CreateRaw("true");
    cJSON_AddItemToObject(root, "raw_flag", raw_item);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    prealloc_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    raw_check = cJSON_IsRaw(raw_item);
    dup_root = cJSON_Duplicate(root, 1);
    printed_dup = cJSON_PrintUnformatted(dup_root);
    equal = cJSON_Compare(root, dup_root, 1);
    cJSON_DeleteItemFromObjectCaseSensitive(dup_root, "raw_flag");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(printed_dup);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    (void)prealloc_ok;
    (void)raw_check;
    (void)equal;
    return 66;
}
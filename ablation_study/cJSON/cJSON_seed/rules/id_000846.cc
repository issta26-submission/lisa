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
//<ID> 846
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *name = NULL;
    cJSON *flag_true = NULL;
    cJSON *raw_meta = NULL;
    cJSON *dup_root = NULL;
    char *printed = NULL;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool raw_check = 0;
    const char *greeting = "hello";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name = cJSON_CreateString(greeting);
    cJSON_AddItemToObject(root, "greeting", name);
    flag_true = cJSON_AddTrueToObject(root, "active");
    raw_meta = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToObject(root, "raw_meta", raw_meta);

    // step 3: Operate / Validate
    dup_root = cJSON_Duplicate(root, 1);
    printed = cJSON_PrintUnformatted(dup_root);
    raw_check = cJSON_IsRaw(cJSON_GetObjectItemCaseSensitive(root, "raw_meta"));

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "greeting");
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    (void)flag_true;
    (void)raw_check;
    return 66;
}
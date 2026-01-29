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
//<ID> 1429
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *dup = NULL;
    cJSON *newobj = NULL;
    cJSON *dup_check = NULL;
    char *printed = NULL;
    char prebuf[256];
    cJSON_bool pre_ok = 0;
    const char json[] = "{\"alpha\":1, \"beta\":[true,false]}";
    const char newjson[] = "{\"gamma\":3}";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    parsed = cJSON_Parse(json);
    dup = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(root, "original", parsed);
    cJSON_AddItemToObject(root, "duplicate", dup);
    newobj = cJSON_Parse(newjson);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "original", newobj);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    memset(prebuf, 0, sizeof(prebuf));
    pre_ok = cJSON_PrintPreallocated(root, prebuf, (int)sizeof(prebuf), 1);
    dup_check = cJSON_GetObjectItem(root, "duplicate");
    (void)dup_check;
    (void)pre_ok;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
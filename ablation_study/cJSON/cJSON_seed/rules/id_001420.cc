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
//<ID> 1420
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
    cJSON *new_item = NULL;
    char *printed = NULL;
    char json_orig[] = "{\"name\":\"original\",\"value\":123}";
    char json_repl[] = "{\"name\":\"replaced\",\"extra\":true}";

    // step 2: Setup / Configure
    parsed = cJSON_Parse(json_orig);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", parsed);
    dup = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(root, "duplicate", dup);

    // step 3: Operate / Validate
    new_item = cJSON_Parse(json_repl);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "child", new_item);
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1428
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
    cJSON *extra_obj = NULL;
    char *printed = NULL;
    cJSON_bool add_ok = 0;
    cJSON_bool replace_ok = 0;
    const char json_text[] = "{\"key\": 123, \"nested\": {\"a\": true}}";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    parsed = cJSON_Parse(json_text);
    add_ok = cJSON_AddItemToObject(root, "parsed", parsed);
    extra_obj = cJSON_CreateObject();
    add_ok = cJSON_AddItemToObject(root, "extra", extra_obj);

    // step 3: Operate / Validate
    dup = cJSON_Duplicate(parsed, 1);
    replace_ok = cJSON_ReplaceItemInObjectCaseSensitive(root, "parsed", dup);
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}
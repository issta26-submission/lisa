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
//<ID> 1260
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char raw_json[] = "{\"deep\":42}";
    cJSON *root = NULL;
    cJSON *nested = NULL;
    cJSON *raw_item = NULL;
    cJSON *payload_lookup = NULL;
    cJSON *nested_lookup = NULL;
    cJSON *enabled_lookup = NULL;
    cJSON *dup = NULL;
    char *printed = NULL;
    cJSON_bool added = 0;
    cJSON_bool is_true = 0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    nested = cJSON_AddObjectToObject(root, "nested");
    cJSON_AddTrueToObject(nested, "enabled");
    raw_item = cJSON_CreateRaw(raw_json);
    added = cJSON_AddItemToObjectCS(root, "payload", raw_item);

    // step 3: Operate / Validate
    payload_lookup = cJSON_GetObjectItemCaseSensitive(root, "payload");
    nested_lookup = cJSON_GetObjectItem(root, "nested");
    enabled_lookup = cJSON_GetObjectItem(nested_lookup, "enabled");
    is_true = cJSON_IsTrue(enabled_lookup);
    printed = cJSON_PrintUnformatted(root);
    dup = cJSON_Duplicate(root, 1);
    equal = cJSON_Compare(root, dup, 1);
    (void)added;
    (void)payload_lookup;
    (void)is_true;
    (void)equal;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
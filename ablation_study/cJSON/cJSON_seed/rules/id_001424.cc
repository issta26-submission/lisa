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
//<ID> 1424
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
    cJSON *replacement = NULL;
    cJSON *inner = NULL;
    char *printed = NULL;
    const char json_in[] = "{\"name\":\"original\",\"value\":123}";

    // step 2: Setup / Configure
    parsed = cJSON_Parse(json_in);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "embedded", parsed);
    dup = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(root, "embedded_copy", dup);
    replacement = cJSON_CreateObject();
    inner = cJSON_CreateString("replaced_name");
    cJSON_AddItemToObject(replacement, "name", inner);
    cJSON_AddItemToObject(replacement, "value", cJSON_CreateNumber(999.0));
    cJSON_ReplaceItemInObjectCaseSensitive(root, "embedded", replacement);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
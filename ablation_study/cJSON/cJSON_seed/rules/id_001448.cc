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
//<ID> 1448
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *child_null = NULL;
    cJSON *replacement = NULL;
    char *printed = NULL;
    cJSON_bool is_null = 0;
    const char raw_json[] = "42";
    const char key_name[] = "my_child";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    child_null = cJSON_CreateNull();
    cJSON_AddItemToObject(child, "inner_null", child_null);
    cJSON_AddItemToObjectCS(root, key_name, child);
    cJSON_AddRawToObject(root, "raw_value", raw_json);

    // step 3: Operate / Validate
    is_null = cJSON_IsNull(child_null);
    replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemViaPointer(root, child, replacement);
    printed = cJSON_Print(root);
    (void)is_null;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
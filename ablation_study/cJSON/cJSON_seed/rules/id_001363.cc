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
//<ID> 1363
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
    cJSON *value_item = NULL;
    cJSON *raw_item = NULL;
    cJSON *replacement = NULL;
    char *raw_text = NULL;
    char *printed = NULL;
    cJSON_bool replace_res = 0;
    cJSON_bool is_raw_res = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("initial_name");
    cJSON_AddItemToObject(root, "name", name_item);
    value_item = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(root, "value", value_item);
    raw_text = (char *)cJSON_malloc(32);
    memset(raw_text, 0, 32);
    memcpy(raw_text, "{\"x\":1}", sizeof("{\"x\":1}"));
    raw_item = cJSON_CreateRaw(raw_text);
    cJSON_AddItemToObject(root, "raw", raw_item);

    // step 3: Operate / Validate
    replacement = cJSON_CreateString("replaced_name");
    replace_res = cJSON_ReplaceItemInObject(root, "name", replacement);
    is_raw_res = cJSON_IsRaw(cJSON_GetObjectItem(root, "raw"));
    cJSON_AddBoolToObject(root, "replace_ok", replace_res);
    cJSON_AddBoolToObject(root, "raw_is_raw", is_raw_res);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(raw_text);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
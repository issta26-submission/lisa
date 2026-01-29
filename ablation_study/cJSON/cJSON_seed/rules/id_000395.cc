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
//<ID> 395
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = NULL;
    cJSON *raw_item = cJSON_CreateRaw("{\"raw_flag\":true}");
    cJSON *str_item = cJSON_CreateString("example");
    cJSON *retrieved_raw = NULL;
    cJSON *retrieved_str = NULL;
    cJSON *bool_item = NULL;
    cJSON_bool flag_val = 0;
    const char *str_val = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON_AddItemToObject(root, "name", str_item);
    cJSON_AddBoolToObject(root, "active", 1);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    retrieved_raw = cJSON_GetObjectItemCaseSensitive(parsed, "raw");
    retrieved_str = cJSON_GetObjectItem(parsed, "name");
    bool_item = cJSON_GetObjectItem(parsed, "active");
    flag_val = cJSON_IsTrue(bool_item);
    str_val = cJSON_GetStringValue(retrieved_str);
    cJSON *copied_str = cJSON_CreateString(str_val ? str_val : "");
    cJSON_AddItemToObject(parsed, "copied", copied_str);
    cJSON *raw_clone = cJSON_CreateRaw("{\"clone\":1}");
    cJSON_AddItemToObject(parsed, "clone_raw", raw_clone);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}
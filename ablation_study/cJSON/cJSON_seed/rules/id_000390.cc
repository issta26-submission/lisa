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
//<ID> 390
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *root = cJSON_CreateObject();
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON *str_item = cJSON_CreateString("example");
    cJSON *bool_item = NULL;
    cJSON *parsed = NULL;
    cJSON *retrieved_raw = NULL;
    cJSON *retrieved_name = NULL;
    cJSON *retrieved_enabled = NULL;
    const char *name_val = NULL;
    cJSON_bool enabled_val = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON_AddItemToObject(root, "name", str_item);
    bool_item = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    retrieved_raw = cJSON_GetObjectItem(parsed, "raw");
    retrieved_name = cJSON_GetObjectItem(parsed, "name");
    retrieved_enabled = cJSON_GetObjectItem(parsed, "enabled");
    name_val = cJSON_GetStringValue(retrieved_name);
    enabled_val = cJSON_IsTrue(retrieved_enabled);
    buffer[0] = (char)('0' + (int)enabled_val);
    buffer[1] = name_val[0];
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}
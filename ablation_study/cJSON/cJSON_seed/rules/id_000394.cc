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
//<ID> 394
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON *str_item = cJSON_CreateString("hello_world");
    cJSON *parsed = NULL;
    cJSON *retrieved_raw = NULL;
    cJSON *retrieved_str = NULL;
    const char *str_val = NULL;

    // step 2: Setup / Configure
    cJSON_AddBoolToObject(root, "active", 1);
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON_AddItemToObject(root, "greeting", str_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    retrieved_raw = cJSON_GetObjectItemCaseSensitive(parsed, "raw");
    retrieved_str = cJSON_GetObjectItemCaseSensitive(parsed, "greeting");
    str_val = cJSON_GetStringValue(retrieved_str);
    buffer[0] = str_val[0];
    buffer[1] = (char)('0' + (int)cJSON_IsRaw(retrieved_raw));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}
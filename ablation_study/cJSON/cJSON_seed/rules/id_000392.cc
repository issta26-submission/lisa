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
//<ID> 392
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
    cJSON *raw_item = cJSON_CreateRaw("raw_payload");
    cJSON *str_item = cJSON_CreateString("greeting");
    cJSON *bool_true_item = NULL;
    cJSON *bool_false_item = NULL;
    cJSON *parsed = NULL;
    cJSON *retrieved_raw = NULL;
    cJSON *retrieved_str = NULL;
    char *retrieved_cstr = NULL;
    cJSON_bool raw_is = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON_AddItemToObject(root, "message", str_item);
    bool_true_item = cJSON_AddBoolToObject(root, "flag_true", 1);
    bool_false_item = cJSON_AddBoolToObject(root, "flag_false", 0);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    retrieved_raw = cJSON_GetObjectItemCaseSensitive(parsed, "raw");
    retrieved_str = cJSON_GetObjectItemCaseSensitive(parsed, "message");
    retrieved_cstr = cJSON_GetStringValue(retrieved_str);
    raw_is = cJSON_IsRaw(retrieved_raw);
    buffer[0] = retrieved_cstr ? retrieved_cstr[0] : '0';
    buffer[1] = (char)('0' + (int)raw_is);
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}
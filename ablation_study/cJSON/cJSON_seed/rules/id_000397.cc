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
//<ID> 397
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(32);
    memset(buffer, 0, 32);
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = NULL;
    cJSON *raw_item = cJSON_CreateRaw("{\"inner\":true}");
    cJSON *str_item = cJSON_CreateString("message");
    cJSON *bool_item_parsed = NULL;
    cJSON *raw_item_parsed = NULL;
    cJSON *str_item_parsed = NULL;
    char *retrieved_str = NULL;
    cJSON_bool is_raw = 0;
    cJSON_bool is_true = 0;

    // step 2: Setup / Configure
    cJSON_AddBoolToObject(root, "ok", 1);
    cJSON_AddItemToObject(root, "payload", raw_item);
    cJSON_AddItemToObject(root, "text", str_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    bool_item_parsed = cJSON_GetObjectItem(parsed, "ok");
    raw_item_parsed = cJSON_GetObjectItem(parsed, "payload");
    str_item_parsed = cJSON_GetObjectItem(parsed, "text");
    is_true = cJSON_IsTrue(bool_item_parsed);
    is_raw = cJSON_IsRaw(raw_item_parsed);
    retrieved_str = cJSON_GetStringValue(str_item_parsed);
    buffer[0] = (char)('0' + (int)is_true);
    buffer[1] = retrieved_str[0];
    buffer[2] = (char)('N' + (is_raw * ('R' - 'N')));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}
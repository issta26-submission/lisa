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
//<ID> 1467
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    const char *json_text = "{\"num\": 123, \"text\": \"hello\"}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *injected = cJSON_CreateString("injected");
    cJSON_AddItemToObject(root, "injected", injected);

    // step 3: Operate & Validate
    cJSON *num_item = cJSON_GetObjectItem(parsed, "num");
    cJSON_bool num_is_number = cJSON_IsNumber(num_item);
    cJSON *retrieved = cJSON_GetObjectItem(root, "injected");
    const char *retrieved_str = cJSON_GetStringValue(retrieved);
    (void)version;
    (void)num_is_number;
    (void)retrieved_str;

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
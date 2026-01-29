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
//<ID> 1449
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *str_item = NULL;
    cJSON *raw_item = NULL;
    cJSON *replacement = NULL;
    cJSON *null_item = NULL;
    char *printed = NULL;
    char prebuf[256];
    cJSON_bool is_null = 0;
    cJSON_bool replace_ok = 0;
    const char raw_json[] = "{\"nested\":true}";
    const char *greeting = "hello world";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString(greeting);
    cJSON_AddItemToObjectCS(root, "greeting", str_item);
    raw_item = cJSON_AddRawToObject(root, "rawdata", raw_json);
    null_item = cJSON_CreateNull();
    replacement = cJSON_CreateNumber(123.0);

    // step 3: Operate / Validate
    is_null = cJSON_IsNull(null_item);
    replace_ok = cJSON_ReplaceItemViaPointer(root, str_item, replacement);
    printed = cJSON_Print(root);
    memset(prebuf, 0, sizeof(prebuf));
    (void)is_null;
    (void)replace_ok;
    (void)raw_item;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(null_item);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
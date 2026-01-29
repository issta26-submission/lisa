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
//<ID> 1326
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *greeting = cJSON_CreateString("hello_world");
    cJSON *flag = cJSON_CreateTrue();
    cJSON *meta = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "greeting", greeting);
    cJSON_AddItemToObjectCS(root, "flag", flag);
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON *inner = cJSON_CreateString("inner_value");
    cJSON_AddItemToObjectCS(meta, "inner", inner);

    // step 3: Operate & Validate
    char *val = cJSON_GetStringValue(greeting);
    cJSON *greeting_copy = cJSON_CreateString(val);
    cJSON_AddItemToObjectCS(root, "greeting_copy", greeting_copy);
    char *printed = cJSON_PrintBuffered(root, 128, 1);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
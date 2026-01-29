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
//<ID> 1322
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *strItem = cJSON_CreateString("hello world");
    cJSON *trueItem = cJSON_CreateTrue();
    cJSON *child = cJSON_CreateObject();
    cJSON *inner = cJSON_CreateString("inner value");

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "greeting", strItem);
    cJSON_AddItemToObjectCS(root, "flag", trueItem);
    cJSON_AddItemToObjectCS(root, "child", child);
    cJSON_AddItemToObjectCS(child, "inner_key", inner);

    // step 3: Operate & Validate
    cJSON *greet_item = cJSON_GetObjectItem(root, "greeting");
    char *greet_cstr = cJSON_GetStringValue(greet_item);
    (void)greet_cstr;
    char *printed = cJSON_PrintBuffered(root, 512, 1);
    size_t len = strlen(printed);
    char *copy = (char *)cJSON_malloc(len + 1);
    memset(copy, 0, len + 1);
    memcpy(copy, printed, len + 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
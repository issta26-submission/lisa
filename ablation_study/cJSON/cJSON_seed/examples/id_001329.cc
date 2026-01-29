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
//<ID> 1329
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *greeting = cJSON_CreateString("hello world");
    cJSON *pi = cJSON_CreateNumber(3.14159);

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "active", true_item);
    cJSON_AddItemToObjectCS(root, "greeting", greeting);
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON_AddItemToObject(meta, "pi", pi);

    // step 3: Operate & Validate
    cJSON *g_item = cJSON_GetObjectItem(root, "greeting");
    char *g_value = cJSON_GetStringValue(g_item);
    cJSON *g_copy = cJSON_CreateString(g_value);
    cJSON_AddItemToObjectCS(root, "greeting_copy", g_copy);
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    size_t printed_len = strlen(printed);
    (void)printed_len; /* use length to demonstrate consumption of printed output */
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
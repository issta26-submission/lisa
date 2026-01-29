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
//<ID> 1328
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
    cJSON *name = cJSON_CreateString("example_name");
    cJSON *value = cJSON_CreateString("example_value");
    cJSON *flag = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToObjectCS(meta, "name", name);
    cJSON_AddItemToObjectCS(meta, "value", value);
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON_AddItemToObjectCS(root, "enabled", flag);

    // step 3: Operate & Validate
    char *name_str = cJSON_GetStringValue(name);
    size_t name_len = strlen(name_str);
    char *name_copy = (char *)cJSON_malloc(name_len + 1);
    memset(name_copy, 0, name_len + 1);
    memcpy(name_copy, name_str, name_len + 1);
    char *printed = cJSON_PrintBuffered(root, 128, 1);
    size_t printed_len = strlen(printed);
    char *snapshot = (char *)cJSON_malloc(printed_len + 1);
    memset(snapshot, 0, printed_len + 1);
    memcpy(snapshot, printed, printed_len + 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(name_copy);
    cJSON_free(snapshot);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
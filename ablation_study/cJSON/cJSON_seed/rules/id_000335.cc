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
//<ID> 335
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *str_item = cJSON_CreateString("example_name");
    cJSON *bool_item = cJSON_CreateTrue();
    cJSON *version_item = cJSON_CreateString(version);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *retrieved_child = NULL;
    cJSON *retrieved_str = NULL;
    cJSON *retrieved_bool = NULL;
    cJSON_bool is_true = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(child, "name", str_item);
    cJSON_AddItemToObject(child, "enabled", bool_item);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "version", version_item);

    // step 3: Operate / Validate
    retrieved_child = cJSON_GetObjectItem(root, "child");
    retrieved_str = cJSON_GetObjectItem(retrieved_child, "name");
    retrieved_bool = cJSON_GetObjectItem(retrieved_child, "enabled");
    is_true = cJSON_IsTrue(retrieved_bool);
    buffer[0] = retrieved_str->valuestring[0];
    buffer[1] = (char)('0' + (int)is_true);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
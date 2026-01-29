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
//<ID> 333
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *flag = cJSON_CreateTrue();
    cJSON *name = cJSON_CreateString("example");
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    const char *version = NULL;
    cJSON *retrieved_child = NULL;
    cJSON *retrieved_flag = NULL;
    cJSON_bool is_enabled = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "enabled", flag);
    cJSON_AddItemToObject(root, "name", name);

    // step 3: Operate / Validate
    retrieved_child = cJSON_GetObjectItem(root, "child");
    retrieved_flag = cJSON_GetObjectItem(retrieved_child, "enabled");
    is_enabled = cJSON_IsTrue(retrieved_flag);
    version = cJSON_Version();
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = (char)('0' + (int)is_enabled);
    buffer[1] = version[0];

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 845
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *str_item = cJSON_CreateString("hello world");

    // step 2: Configure
    cJSON_AddItemToObject(root, "text", str_item);
    cJSON_AddItemToObject(root, "child", child);
    cJSON *title = cJSON_AddStringToObject(child, "title", "nested title");

    // step 3: Operate & Validate
    char *extracted = cJSON_GetStringValue(title);
    cJSON_AddStringToObject(root, "echo", extracted);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
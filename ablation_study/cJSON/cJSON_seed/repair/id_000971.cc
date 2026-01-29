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
//<ID> 971
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_AddObjectToObject(root, "config");
    cJSON *sub = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(cfg, "sub_CS", sub);

    // step 2: Configure
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "name", "example");
    cJSON_AddNumberToObject(child, "value", 42.0);
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromObject(root, "child");
    cJSON_AddItemToObjectCS(cfg, "detached_child", detached);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    cJSON *got = cJSON_GetObjectItem(cfg, "detached_child");
    const char *name = cJSON_GetStringValue(cJSON_GetObjectItem(got, "name"));
    double val = cJSON_GetNumberValue(cJSON_GetObjectItem(got, "value"));
    (void)name;
    (void)val;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
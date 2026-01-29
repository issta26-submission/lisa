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
//<ID> 2460
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
    cJSON *true_item = cJSON_AddTrueToObject(child, "active");
    cJSON_AddItemToObject(root, "child", child);
    cJSON *num = cJSON_CreateNumber(2026.0);
    cJSON_AddItemToObject(root, "year", num);
    cJSON *f = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", f);
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name);

    // step 2: Configure
    char *printed = cJSON_Print(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';
    buffer[1] = (char)('A' + (int)cJSON_GetNumberValue(num) % 26);

    // step 3: Operate
    cJSON_bool is_enabled_false = cJSON_IsFalse(f);
    buffer[2] = (char)(int)is_enabled_false;
    cJSON_bool is_active_false = cJSON_IsFalse(true_item);
    buffer[3] = (char)(int)is_active_false;
    char *repr = cJSON_Print(root);
    buffer[4] = repr && repr[0] ? repr[0] : '\0';

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(repr);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
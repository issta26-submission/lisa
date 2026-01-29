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
//<ID> 640
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
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "greeting", "hello world");
    cJSON_AddStringToObject(root, "direct", "direct-value");
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    cJSON *g_item = cJSON_GetObjectItem(meta, "greeting");
    const char *gstr = cJSON_GetStringValue(g_item);
    cJSON_AddStringToObject(root, "echo", gstr);
    cJSON_bool flag_is_false = cJSON_IsFalse(flag);
    cJSON_AddNumberToObject(root, "flag_is_false", (double)flag_is_false);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)printed_ok;
    cJSON_AddStringToObject(root, "snapshot", buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
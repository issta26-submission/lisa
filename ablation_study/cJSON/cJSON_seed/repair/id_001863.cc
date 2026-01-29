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
//<ID> 1863
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
    cJSON *flag_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta, "flag", flag_item);
    cJSON *ver_item = cJSON_CreateString("1.0");
    cJSON_AddItemToObject(meta, "version", ver_item);
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *ans = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(data, "answer", ans);

    // step 2: Configure
    char *set_ret = cJSON_SetValuestring(ver_item, "2.0");
    (void)set_ret;
    cJSON *extra = cJSON_CreateString("extra_info");
    cJSON_AddItemToObject(data, "info", extra);

    // step 3: Operate and Validate
    cJSON *got_flag = cJSON_GetObjectItem(meta, "flag");
    int flag_true = cJSON_IsTrue(got_flag);
    const char *got_ver = cJSON_GetStringValue(ver_item);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed[0];
    buffer[1] = got_ver[0];
    buffer[2] = (char)('0' + flag_true);
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
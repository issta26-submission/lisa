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
//<ID> 1911
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *status = cJSON_CreateString("init");
    cJSON_AddItemToObject(data, "status", status);
    cJSON *count = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(data, "count", count);

    // step 2: Configure
    char *ret_set = cJSON_SetValuestring(status, "running");
    cJSON *new_count = cJSON_CreateNumber(42.0);
    cJSON_bool replaced = cJSON_ReplaceItemInObject(data, "count", new_count);

    // step 3: Operate and Validate
    cJSON *got_status = cJSON_GetObjectItem(data, "status");
    cJSON *got_count = cJSON_GetObjectItem(data, "count");
    char *s = cJSON_GetStringValue(got_status);
    double v = cJSON_GetNumberValue(got_count);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + ((s && s[0]) ? ((unsigned char)s[0] % 10) : 0));
    buffer[2] = (char)('0' + (((int)v) % 10));
    buffer[3] = (char)('0' + (replaced % 10));
    buffer[4] = ret_set ? ret_set[0] : 'X';
    buffer[5] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
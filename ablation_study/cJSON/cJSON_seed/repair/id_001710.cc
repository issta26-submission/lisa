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
//<ID> 1710
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, flag_true);
    cJSON *num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(items, num);
    cJSON *str = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, str);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "status", "ok");
    cJSON_AddNumberToObject(meta, "id", 101.0);

    // step 3: Operate and Validate
    cJSON_bool is_arr = cJSON_IsArray(items);
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    char *printed = cJSON_PrintUnformatted(root);
    char *buf = (char*)cJSON_malloc(64);
    memset(buf, 0, 64);
    buf[0] = printed[0];
    buf[1] = (char)('0' + (int)is_arr);
    cJSON *status_item = cJSON_GetObjectItem(got_meta, "status");
    char *status_val = cJSON_GetStringValue(status_item);
    buf[2] = status_val ? status_val[0] : 'Z';
    (void)got_items;
    (void)got_meta;
    (void)status_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}
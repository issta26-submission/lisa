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
//<ID> 1960
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
    cJSON *label = cJSON_CreateString("init_label");
    cJSON_AddItemToObject(meta, "label", label);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(meta, "ok", flag);

    // step 2: Configure
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *first_item = cJSON_CreateObject();
    cJSON_AddItemToArray(items, first_item);
    cJSON *name = cJSON_CreateString("first");
    cJSON_AddItemToObject(first_item, "name", name);

    // step 3: Operate and Validate
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_label = cJSON_GetObjectItem(got_meta, "label");
    const char *label_str = cJSON_GetStringValue(got_label);
    int flag_is_false = (int)cJSON_IsFalse(flag);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = (char)('0' + (flag_is_false % 10));
    buffer[1] = (label_str && label_str[0]) ? label_str[0] : 'x';
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
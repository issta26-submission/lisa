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
//<ID> 1958
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "payload", payload);
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(payload, "list", list);
    cJSON *item1 = cJSON_CreateString("alpha");
    cJSON *item2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(list, item1);
    cJSON_AddItemToArray(list, item2);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    const char *ver = cJSON_Version();
    cJSON *ver_item = cJSON_CreateString(ver ? ver : "unknown");
    cJSON_AddItemToObject(meta, "version", ver_item);
    cJSON *label = cJSON_CreateString("primary");
    cJSON_AddItemToObject(meta, "label", label);

    // step 3: Operate and Validate
    cJSON *got_payload = cJSON_GetObjectItem(root, "payload");
    cJSON_bool payload_is_obj = cJSON_IsObject(got_payload);
    cJSON *got_list = cJSON_GetObjectItem(got_payload, "list");
    cJSON_bool list_is_invalid = cJSON_IsInvalid(got_list);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    cJSON *got_label = cJSON_GetObjectItem(meta, "label");
    char *label_val = cJSON_GetStringValue(got_label);
    buffer[0] = label_val ? label_val[0] : 'X';
    buffer[1] = (char)('0' + (payload_is_obj % 10));
    buffer[2] = (char)('0' + (list_is_invalid % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1950
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
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *label = cJSON_CreateString("init_label");
    cJSON_AddItemToObject(meta, "label", label);
    cJSON *num = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(meta, "pi", num);
    cJSON *elem1 = cJSON_CreateString("first");
    cJSON *elem2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(items, elem1);
    cJSON_AddItemToArray(items, elem2);

    // step 2: Configure
    cJSON *version_item = cJSON_CreateString("unknown");
    cJSON_AddItemToObject(root, "lib_version", version_item);
    cJSON_SetValuestring(version_item, cJSON_Version());
    cJSON_bool root_obj = cJSON_IsObject(root);
    cJSON_bool ver_invalid = cJSON_IsInvalid(version_item);

    // step 3: Operate and Validate
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    cJSON *got_label = cJSON_GetObjectItem(meta, "label");
    char *got_label_str = cJSON_GetStringValue(got_label);
    buffer[0] = got_label_str ? got_label_str[0] : 'X';
    buffer[1] = (char)('0' + (root_obj % 10));
    buffer[2] = (char)('0' + (ver_invalid % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
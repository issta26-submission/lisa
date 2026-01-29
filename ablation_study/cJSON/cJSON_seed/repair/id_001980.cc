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
//<ID> 1980
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
    cJSON *num_item = cJSON_CreateNumber(42.5);
    cJSON *str_item = cJSON_CreateString("payload");
    cJSON_AddItemToObject(child, "value", num_item);
    cJSON_AddItemToObject(child, "text", str_item);
    cJSON_AddItemToObject(root, "child", child);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "ok", flag);

    // step 2: Configure
    cJSON *ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", ref);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);

    // step 3: Operate and Validate
    const char *text_val = cJSON_GetStringValue(cJSON_GetObjectItem(child, "text"));
    double number_val = cJSON_GetNumberValue(cJSON_GetObjectItem(child, "value"));
    cJSON_bool flag_is_false = cJSON_IsFalse(flag);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = (char)('0' + (printed_ok % 10));
    buffer[1] = (char)('0' + (flag_is_false % 10));
    buffer[2] = (text_val && text_val[0]) ? text_val[0] : 'x';
    buffer[3] = (char)('0' + ((int)number_val % 10));
    buffer[4] = '\0';
    (void)number_val;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
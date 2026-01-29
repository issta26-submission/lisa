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
//<ID> 1989
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
    cJSON *num_item = cJSON_CreateNumber(3.14);
    cJSON *str_item = cJSON_CreateString("example");
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(child, "value", num_item);
    cJSON_AddItemToObject(child, "label", str_item);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "ok", flag);

    // step 2: Configure
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);
    cJSON *extra = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", extra);

    // step 3: Operate and Validate
    cJSON *got_child = cJSON_GetObjectItem(root, "child");
    cJSON *got_num = cJSON_GetObjectItem(got_child, "value");
    double val = cJSON_GetNumberValue(got_num);
    cJSON *got_label = cJSON_GetObjectItem(got_child, "label");
    char *label = cJSON_GetStringValue(got_label);
    cJSON_bool is_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "ok"));
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = (char)('0' + (printed_ok % 10));
    buffer[1] = (char)('0' + ((int)is_false % 10));
    buffer[2] = (label && label[0]) ? label[0] : 'x';
    buffer[3] = (char)('0' + ((int)val % 10));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
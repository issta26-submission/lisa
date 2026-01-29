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
//<ID> 1992
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
    cJSON *num_item = cJSON_CreateNumber(123.0);
    cJSON *name_item = cJSON_CreateString("initial_name");
    cJSON_AddItemToObject(child, "value", num_item);
    cJSON_AddItemToObject(child, "name", name_item);
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *false_flag = cJSON_AddFalseToObject(root, "ok");
    cJSON *maybe_null = cJSON_AddNullToObject(root, "maybe");
    cJSON *addr_item = cJSON_CreateString("addr_old");
    cJSON_AddItemToObject(root, "addr", addr_item);
    char *new_addr = cJSON_SetValuestring(addr_item, "addr_new");
    const char *version = cJSON_Version();

    // step 3: Operate and Validate
    cJSON *got_child = cJSON_GetObjectItem(root, "child");
    cJSON *got_name = cJSON_GetObjectItem(got_child, "name");
    const char *name_str = cJSON_GetStringValue(got_name);
    cJSON *got_value = cJSON_GetObjectItem(got_child, "value");
    double value_num = cJSON_GetNumberValue(got_value);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = (char)('0' + (printed_ok % 10));
    buffer[1] = (version && version[0]) ? version[0] : 'x';
    buffer[2] = (name_str && name_str[0]) ? name_str[0] : 'x';
    buffer[3] = (new_addr && new_addr[0]) ? new_addr[0] : 'x';
    buffer[4] = (char)('0' + ((int)value_num % 10));
    buffer[5] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
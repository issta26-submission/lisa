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
//<ID> 1952
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
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON *item1 = cJSON_CreateString("first");
    cJSON_AddItemToArray(list, item1);
    cJSON *item2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(list, item2);
    cJSON *label = cJSON_CreateString("root_label");
    cJSON_AddItemToObject(data, "label", label);

    // step 2: Configure
    const char *version = cJSON_Version();
    cJSON *got_data = cJSON_GetObjectItem(root, "data");
    cJSON *got_label = cJSON_GetObjectItem(got_data, "label");
    cJSON_bool is_obj = cJSON_IsObject(got_data);
    cJSON_bool is_invalid = cJSON_IsInvalid(got_label);

    // step 3: Operate and Validate
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = version && version[0] ? version[0] : 'X';
    buffer[1] = (char)('0' + (is_obj % 10));
    buffer[2] = (char)('0' + (is_invalid % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
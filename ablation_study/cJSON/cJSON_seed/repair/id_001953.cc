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
//<ID> 1953
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *obj = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(obj, "name", name);
    cJSON_bool add_ok = cJSON_AddItemToArray(arr, obj);

    // step 2: Configure
    cJSON *got_name_item = cJSON_GetObjectItem(obj, "name");
    const char *version = cJSON_Version();
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);

    // step 3: Operate and Validate
    cJSON_bool root_is_object = cJSON_IsObject(root);
    cJSON_bool name_is_invalid = cJSON_IsInvalid(got_name_item);
    const char *name_str = cJSON_GetStringValue(got_name_item);
    buffer[0] = version[0];
    buffer[1] = name_str[0];
    buffer[2] = (char)('0' + (root_is_object % 10));
    buffer[3] = (char)('0' + (name_is_invalid % 10));
    buffer[4] = (char)('0' + (add_ok % 10));
    buffer[5] = (char)('0' + (printed % 10));
    buffer[6] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
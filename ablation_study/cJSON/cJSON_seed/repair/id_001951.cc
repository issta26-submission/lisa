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
//<ID> 1951
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *label = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(meta, "label", label);
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(data, num);
    cJSON *str = cJSON_CreateString("hello");
    cJSON_AddItemToArray(data, str);

    // step 2: Configure
    cJSON_SetValuestring(label, "beta");
    const char *version = cJSON_Version();

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *got_data = cJSON_GetObjectItem(root, "data");
    cJSON_bool got_data_is_object = cJSON_IsObject(got_data);
    cJSON_bool label_is_invalid = cJSON_IsInvalid(label);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (got_data_is_object % 10));
    buffer[2] = (char)('0' + (label_is_invalid % 10));
    buffer[3] = version ? version[0] : 'V';
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
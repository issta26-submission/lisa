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
//<ID> 1940
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *label = cJSON_CreateString("initial_label");
    cJSON_AddItemToObject(meta, "label", label);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(meta, "flag", flag);

    // step 2: Configure
    cJSON_SetValuestring(label, "renamed_label");

    // step 3: Operate and Validate
    char *json_str = cJSON_PrintUnformatted(root);
    const int buflen = 4;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (char)('0' + (cJSON_IsFalse(flag) % 10));
    buffer[1] = (char)('0' + (cJSON_IsArray(arr) % 10));
    buffer[2] = json_str[0];
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(json_str);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
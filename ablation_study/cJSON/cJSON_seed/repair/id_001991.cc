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
//<ID> 1991
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *name = cJSON_CreateString("initial_name");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *flag_false = cJSON_AddFalseToObject(root, "active");
    cJSON *nul = cJSON_AddNullToObject(root, "missing");

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(config, "retry", num);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *s2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);

    // step 3: Operate and Validate
    char *updated_ptr = cJSON_SetValuestring(name, "updated_name");
    const char *ver = cJSON_Version();
    char *json_out = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = updated_ptr[0];
    buffer[1] = ver[0];
    buffer[2] = json_out[0];
    int asz = cJSON_GetArraySize(arr);
    buffer[3] = (char)('0' + (asz % 10));
    cJSON_bool is_false = cJSON_IsFalse(flag_false);
    buffer[4] = (char)('0' + ((int)is_false % 10));
    buffer[5] = '\0';

    // step 4: Cleanup
    cJSON_free(json_out);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
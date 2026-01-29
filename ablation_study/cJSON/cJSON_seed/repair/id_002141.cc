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
//<ID> 2141
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data_array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", data_array);

    // step 2: Configure
    cJSON *greeting = cJSON_AddStringToObject(root, "greeting", "hello_world");
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(root, "answer", num);
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddStringToObject(nested, "inner", "value_inner");
    cJSON_AddItemToObject(root, "nested", nested);
    cJSON *arr_elem = cJSON_CreateString("elem0");
    cJSON_AddItemToArray(data_array, arr_elem);

    // step 3: Operate and Validate
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON *dup_greeting = cJSON_GetObjectItem(dup_root, "greeting");
    const char *gstr = cJSON_GetStringValue(dup_greeting);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = gstr ? gstr[0] : '\0';
    buffer[1] = (char)('0' + (int)cJSON_IsArray(cJSON_GetObjectItem(dup_root, "data")));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
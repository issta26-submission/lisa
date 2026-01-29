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
//<ID> 2146
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

    // step 2: Configure
    cJSON *s = cJSON_CreateString("value1");
    cJSON_AddItemToArray(data, s);
    cJSON_AddStringToObject(root, "greeting", "hello");
    cJSON *dup = cJSON_Duplicate(s, 1);
    cJSON_AddItemToObject(root, "dup_str", dup);

    // step 3: Operate and Validate
    cJSON *fetched = cJSON_GetObjectItem(root, "data");
    cJSON_bool is_arr = cJSON_IsArray(fetched);
    cJSON *flag = cJSON_CreateNumber((double)is_arr);
    cJSON_AddItemToObject(root, "is_array_flag", flag);
    const char *gstr = cJSON_GetStringValue(cJSON_GetObjectItem(root, "greeting"));
    const int buflen = 16;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = gstr ? gstr[0] : '\0';
    buffer[1] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
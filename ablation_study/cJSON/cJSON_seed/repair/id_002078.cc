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
//<ID> 2078
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *count = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "count", count);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON *inner = cJSON_CreateString("inner_value");
    cJSON_AddItemToObject(child, "inner", inner);

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, dup, 1);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *got_name = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(got_name);
    cJSON *got_count = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(got_count);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = name_str[0];
    buffer[2] = (char)('0' + ((int)count_val % 10));
    buffer[3] = (char)(equal ? '1' : '0');
    buffer[4] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 2142
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
    cJSON_AddItemToObject(root, "myarray", arr);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddStringToObject(root, "greeting", "hello_world");
    cJSON *entry = cJSON_CreateString("entry");
    cJSON_AddItemToArray(arr, entry);

    // step 3: Operate and Validate
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON *fetched_arr = cJSON_GetObjectItemCaseSensitive(dup, "myarray");
    cJSON_bool is_array = cJSON_IsArray(fetched_arr);
    char *printed = cJSON_PrintUnformatted(dup);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = printed[1];
    buffer[2] = (char)('0' + (is_array ? 1 : 0));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
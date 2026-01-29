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
//<ID> 2437
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON *str = cJSON_CreateString("detached_string");
    cJSON_AddItemToObjectCS(root, "number", num);
    cJSON_AddItemToObjectCS(root, "text", str);
    cJSON *replacement = cJSON_CreateNumber(456.0);

    // step 2: Configure
    char *printed1 = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed1 && printed1[0] ? printed1[0] : '\0';

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromObject(root, "text");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, num, replacement);
    char *detached_strval = cJSON_GetStringValue(detached);
    buffer[1] = detached_strval && detached_strval[0] ? detached_strval[0] : '\0';
    buffer[2] = (char)(int)replaced;

    // step 4: Validate and Cleanup
    char *printed2 = cJSON_PrintUnformatted(root);
    buffer[3] = printed2 && printed2[0] ? printed2[0] : '\0';
    cJSON_free(printed1);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
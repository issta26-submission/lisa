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
//<ID> 2435
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *num2 = cJSON_CreateNumber(20.5);
    cJSON *str = cJSON_CreateString("hello");
    cJSON_AddItemToObjectCS(root, "n1", num1);
    cJSON_AddItemToObjectCS(root, "n2", num2);
    cJSON_AddItemToObjectCS(root, "s", str);

    // step 2: Configure
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);

    // step 3: Operate
    cJSON *old_n1 = cJSON_GetObjectItem(root, "n1");
    cJSON *replacement = cJSON_CreateNumber(99.9);
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, old_n1, replacement);
    cJSON *detached_s = cJSON_DetachItemFromObject(root, "s");
    char *sval = cJSON_GetStringValue(detached_s);
    buffer[0] = sval && sval[0] ? sval[0] : '\0';
    buffer[1] = (char)(int)cJSON_GetNumberValue(cJSON_GetObjectItem(root, "n2"));
    (void)replaced; // preserve realistic usage of returned value without branching

    // step 4: Validate and Cleanup
    cJSON_Delete(detached_s);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
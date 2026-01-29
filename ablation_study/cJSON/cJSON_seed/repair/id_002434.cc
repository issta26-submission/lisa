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
//<ID> 2434
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num1 = cJSON_CreateNumber(10.5);
    cJSON *num2 = cJSON_CreateNumber(20.25);
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObjectCS(root, "one", num1);
    cJSON_AddItemToObjectCS(root, "two", num2);
    cJSON_AddItemToObjectCS(root, "name", name);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';
    cJSON *detached = cJSON_DetachItemFromObject(root, "two");
    cJSON *replacement = cJSON_CreateNumber(99.99);
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, num1, replacement);
    const char *sval = cJSON_GetStringValue(name);
    buffer[1] = sval && sval[0] ? sval[0] : '\0';
    buffer[2] = (char)(int)replaced;

    // step 3: Operate
    cJSON_AddItemToObjectCS(root, "two_restored", detached);
    char *printed2 = cJSON_PrintUnformatted(root);
    buffer[3] = printed2 && printed2[0] ? printed2[0] : '\0';
    double rep_val = cJSON_GetNumberValue(replacement);
    buffer[4] = (char)(int)rep_val;

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
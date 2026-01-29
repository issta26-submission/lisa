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
//<ID> 2433
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num1 = cJSON_CreateNumber(1.23);
    cJSON *num2 = cJSON_CreateNumber(4.56);
    cJSON *str = cJSON_CreateString("label");
    cJSON_AddItemToObjectCS(root, "a", num1);
    cJSON_AddItemToObjectCS(root, "b", num2);
    cJSON_AddItemToObjectCS(root, "name", str);

    // step 2: Configure
    char *printed1 = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    char *name_val = cJSON_GetStringValue(str);
    buffer[0] = name_val && name_val[0] ? name_val[0] : '\0';
    buffer[1] = printed1 && printed1[0] ? printed1[0] : '\0';

    // step 3: Operate
    cJSON *detached_b = cJSON_DetachItemFromObject(root, "b");
    cJSON *num3 = cJSON_CreateNumber(9.0);
    cJSON *item_a = cJSON_GetObjectItem(root, "a");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, item_a, num3);
    (void)replaced;

    // step 4: Validate and Cleanup
    char *printed2 = cJSON_PrintUnformatted(root);
    buffer[2] = printed2 && printed2[0] ? printed2[0] : '\0';
    cJSON_free(printed1);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(detached_b);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
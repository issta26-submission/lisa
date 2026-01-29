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
//<ID> 2439
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON *str = cJSON_CreateString("hello");
    cJSON_AddItemToObjectCS(child, "num", num);
    cJSON_AddItemToObjectCS(child, "text", str);
    cJSON_AddItemToObjectCS(root, "child", child);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromObject(root, "child");
    cJSON_AddItemToObjectCS(root, "child", detached);
    cJSON *current_num = cJSON_GetObjectItem(detached, "num");
    cJSON *newnum = cJSON_CreateNumber(456.0);
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(detached, current_num, newnum);
    buffer[1] = (char)(int)replaced;

    // step 4: Validate and Cleanup
    cJSON *text_item = cJSON_GetObjectItem(detached, "text");
    char *text_val = cJSON_GetStringValue(text_item);
    buffer[2] = text_val && text_val[0] ? text_val[0] : '\0';
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
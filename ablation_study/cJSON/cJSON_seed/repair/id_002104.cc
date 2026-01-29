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
//<ID> 2104
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_AddObjectToObject(root, "child");
    cJSON *num = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(num, 314.159);
    cJSON_AddItemToObject(child, "num", num);
    cJSON_AddStringToObject(child, "greet", "hello");

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddStringToObject(meta, "name", "cJSON_test");
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromObject(root, "child");
    cJSON *num_item = cJSON_GetObjectItem(detached, "num");
    double numval = cJSON_GetNumberValue(num_item);
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed ? printed[0] : '\0';
    buffer[1] = (char)('0' + ((int)numval % 10));
    buffer[2] = '\0';
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
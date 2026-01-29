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
//<ID> 2106
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
    cJSON_AddItemToObject(child, "initial", num);
    cJSON *nested = cJSON_AddObjectToObject(child, "nested");
    cJSON *s = cJSON_CreateString("nested_name");
    cJSON_AddItemToObject(nested, "label", s);

    // step 2: Configure
    cJSON *num2 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(root, "value", num2);
    cJSON_SetNumberHelper(num2, 42.0);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromObject(child, "initial");
    double detached_val = cJSON_GetNumberValue(detached);
    char *printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + (int)detached_val);
    const char *label = cJSON_GetStringValue(cJSON_GetObjectItem(nested, "label"));
    buffer[2] = label[0];
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
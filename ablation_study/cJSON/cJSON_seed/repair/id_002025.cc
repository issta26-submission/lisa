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
//<ID> 2025
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
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "list", array);
    cJSON_AddNumberToObject(child, "temp", 36.6);
    cJSON_AddTrueToObject(child, "active");

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *n1 = cJSON_CreateNumber(7.0);
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_InsertItemInArray(array, 0, s1);
    cJSON_InsertItemInArray(array, 1, n1);
    cJSON_InsertItemInArray(array, 2, s2);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed ? printed[0] : 'x';
    buffer[1] = cJSON_GetStringValue(cJSON_GetArrayItem(array, 0))[0];
    buffer[2] = (char)('0' + (int)cJSON_GetNumberValue(cJSON_GetObjectItem(child, "temp")) % 10);
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
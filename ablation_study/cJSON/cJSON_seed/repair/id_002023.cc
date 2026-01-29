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
//<ID> 2023
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "version", 3.1415);
    cJSON_AddTrueToObject(root, "active");
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);
    cJSON *item0 = cJSON_CreateString("zero");
    cJSON *item2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(array, item0);
    cJSON_AddItemToArray(array, item2);
    cJSON *item1 = cJSON_CreateString("one");
    cJSON_InsertItemInArray(array, 1, item1);

    // step 2: Configure
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(obj, "score", 42.0);
    cJSON_AddItemToArray(array, obj);

    // step 3: Operate and Validate
    int arr_size = cJSON_GetArraySize(array);
    const char *s0 = cJSON_GetStringValue(cJSON_GetArrayItem(array, 0));
    const char *s1 = cJSON_GetStringValue(cJSON_GetArrayItem(array, 1));
    const char *s2 = cJSON_GetStringValue(cJSON_GetArrayItem(array, 2));
    double ver = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "version"));
    char *printed = cJSON_Print(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = s0[0];
    buffer[2] = s1[0];
    buffer[3] = s2[0];
    buffer[4] = (char)('0' + ((int)ver % 10));
    buffer[5] = printed[0];
    buffer[6] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
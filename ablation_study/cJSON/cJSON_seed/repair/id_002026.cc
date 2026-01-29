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
//<ID> 2026
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "version", 3.14159);
    cJSON_AddTrueToObject(root, "enabled");
    cJSON *elem0 = cJSON_CreateString("first");
    cJSON *elem2 = cJSON_CreateString("third");
    cJSON_AddItemToArray(array, elem0);
    cJSON_AddItemToArray(array, elem2);
    cJSON *elem1 = cJSON_CreateString("second");
    cJSON_InsertItemInArray(array, 1, elem1);

    // step 3: Operate and Validate
    int arr_size = cJSON_GetArraySize(array);
    cJSON *got0 = cJSON_GetArrayItem(array, 0);
    cJSON *got1 = cJSON_GetArrayItem(array, 1);
    cJSON *got2 = cJSON_GetArrayItem(array, 2);
    double ver = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "version"));
    const char *s0 = cJSON_GetStringValue(got0);
    const char *s1 = cJSON_GetStringValue(got1);
    const char *s2 = cJSON_GetStringValue(got2);
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (printed && printed[0]) ? printed[0] : 'x';
    buffer[1] = (s0 && s0[0]) ? s0[0] : 'a';
    buffer[2] = (s1 && s1[0]) ? s1[0] : 'b';
    buffer[3] = (s2 && s2[0]) ? s2[0] : 'c';
    buffer[4] = (char)('0' + (arr_size % 10));
    buffer[5] = (char)('0' + (((int)ver) % 10));
    buffer[6] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
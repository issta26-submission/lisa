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
//<ID> 2030
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
    cJSON *elem0 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(array, elem0);
    cJSON *elem1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(array, elem1);

    // step 2: Configure
    cJSON_AddTrueToObject(root, "active");
    cJSON_AddStringToObject(root, "mode", "old");
    cJSON *replacement_mode = cJSON_CreateString("new");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "mode", replacement_mode);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *mode_item = cJSON_GetObjectItem(root, "mode");
    const char *mode_str = cJSON_GetStringValue(mode_item);
    int arr_size = cJSON_GetArraySize(array);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = mode_str[0];
    buffer[2] = (char)('0' + (arr_size % 10));
    char *minifiable = (char *)cJSON_malloc(16);
    memset(minifiable, 0, (size_t)16);
    minifiable[0] = ' ';
    minifiable[1] = '{';
    minifiable[2] = ' ';
    minifiable[3] = '"';
    minifiable[4] = 'a';
    minifiable[5] = '"';
    minifiable[6] = ' ';
    minifiable[7] = ':';
    minifiable[8] = ' ';
    minifiable[9] = '1';
    minifiable[10] = ' ';
    minifiable[11] = '}';
    minifiable[12] = ' ';
    minifiable[13] = '\0';
    cJSON_Minify(minifiable);
    buffer[3] = minifiable[0];

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_free(minifiable);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
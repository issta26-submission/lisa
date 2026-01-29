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
//<ID> 2039
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
    cJSON_AddStringToObject(root, "label", "original");

    // step 2: Configure
    char json_buf[] = " { \"note\" : \"parsed note\" } ";
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    cJSON *note_item = cJSON_GetObjectItem(parsed, "note");
    const char *note_str = cJSON_GetStringValue(note_item);
    cJSON *arr_item = cJSON_CreateString(note_str);
    cJSON_AddItemToArray(array, arr_item);
    cJSON_AddTrueToObject(root, "flag");
    cJSON *replacement = cJSON_CreateString(note_str);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "label", replacement);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = cJSON_GetStringValue(cJSON_GetArrayItem(array, 0))[0];
    buffer[2] = note_str[0];
    buffer[3] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
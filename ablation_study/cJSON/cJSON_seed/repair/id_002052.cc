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
//<ID> 2052
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
    cJSON_AddNumberToObject(root, "count", 1.0);
    cJSON *first_elem = cJSON_CreateString("elem0");
    cJSON_AddItemToArray(array, first_elem);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", false_item);

    // step 2: Configure
    char json_buf[] = " { \"note\" : \"minify me\" } ";
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    cJSON *note_item = cJSON_GetObjectItem(parsed, "note");
    const char *note_str = cJSON_GetStringValue(note_item);
    cJSON *note_elem = cJSON_CreateString(note_str);
    cJSON_AddItemToArray(array, note_elem);
    cJSON_SetNumberHelper(root, 2.0);

    // step 3: Operate and Validate
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    char *printed = cJSON_PrintUnformatted(dup_root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    const char *arr_first_str = cJSON_GetStringValue(cJSON_GetArrayItem(array, 0));
    buffer[0] = printed[0];
    buffer[1] = arr_first_str[0];
    buffer[2] = note_str[0];
    buffer[3] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 2056
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddStringToObject(root, "label", "original");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", false_item);

    // step 2: Configure
    char json_buf[] = "   { \"note\" : \"minified note\" }   ";
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    cJSON *note_item = cJSON_GetObjectItem(parsed, "note");
    const char *note_str = cJSON_GetStringValue(note_item);
    cJSON *dup = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(root, "parsed_copy", dup);
    cJSON_AddItemToArray(arr, cJSON_CreateString(note_str));

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    const char *first_elem = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 0));
    buffer[1] = first_elem[0];
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
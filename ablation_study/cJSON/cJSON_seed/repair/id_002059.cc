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
//<ID> 2059
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddStringToObject(root, "label", "original_label");
    cJSON_AddItemToObject(root, "alive", cJSON_CreateFalse());
    cJSON *elem0 = cJSON_CreateString("elem0");
    cJSON_AddItemToArray(items, elem0);

    // step 2: Configure
    char json_buf[] = " { \"note\" : \"hello world\" , \"value\": 3.14 } ";
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    cJSON *note = cJSON_GetObjectItem(parsed, "note");
    const char *note_str = cJSON_GetStringValue(note);
    cJSON *note_dup = cJSON_Duplicate(note, 1);
    cJSON_AddItemToArray(items, note_dup);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    const char *first_item_str = cJSON_GetStringValue(cJSON_GetArrayItem(items, 0));
    buffer[0] = printed[0];
    buffer[1] = first_item_str[0];
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
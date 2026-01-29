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
//<ID> 2050
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
    cJSON_AddStringToObject(root, "title", "original");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "active", false_item);
    cJSON *num_item = cJSON_CreateNumber(10.5);
    cJSON_AddItemToArray(items, cJSON_CreateString("elem0"));
    cJSON_AddItemToArray(items, num_item);

    // step 2: Configure
    char json_buf[] = " { \"note\" : \"parsed note\", \"count\" : 7 } ";
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    cJSON *note = cJSON_GetObjectItem(parsed, "note");
    const char *note_str = cJSON_GetStringValue(note);
    cJSON *note_copy = cJSON_CreateString(note_str);
    cJSON_AddItemToArray(items, note_copy);
    cJSON *count = cJSON_GetObjectItem(parsed, "count");
    double count_val = cJSON_GetNumberValue(count);
    cJSON *incremented = cJSON_CreateNumber(count_val + 1.0);
    cJSON_AddItemToObject(root, "incremented_count", incremented);
    cJSON *dup_root = cJSON_Duplicate(root, (cJSON_bool)1);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    const char *first_elem = cJSON_GetStringValue(cJSON_GetArrayItem(items, 0));
    buffer[0] = printed[0];
    buffer[1] = first_elem[0];
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
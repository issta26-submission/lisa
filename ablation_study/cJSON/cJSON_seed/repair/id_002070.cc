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
//<ID> 2070
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name_item = cJSON_CreateString("sample_name");
    cJSON_AddItemToObject(meta, "name", name_item);
    cJSON *count_item = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(meta, "count", count_item);

    // step 2: Configure
    const char raw_json[] = "{\"note\":\"hello\",\"value\":2.718}";
    cJSON *parsed = cJSON_Parse(raw_json);
    cJSON *note = cJSON_GetObjectItem(parsed, "note");
    cJSON *value = cJSON_GetObjectItem(parsed, "value");
    cJSON *note_dup = cJSON_Duplicate(note, 1);
    cJSON_AddItemToObject(root, "note_copy", note_dup);
    cJSON_AddItemToObject(root, "parsed_value", cJSON_CreateNumber(cJSON_GetNumberValue(value)));

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    const char *name_str = cJSON_GetStringValue(cJSON_GetObjectItem(meta, "name"));
    double count_val = cJSON_GetNumberValue(cJSON_GetObjectItem(meta, "count"));
    const char *note_str = cJSON_GetStringValue(note);
    double parsed_val = cJSON_GetNumberValue(value);
    cJSON_bool cmp = cJSON_Compare(note, note_dup, 1);
    buffer[0] = printed[0];
    buffer[1] = name_str[0];
    buffer[2] = note_str[0];
    buffer[3] = (char)('0' + ((int)count_val % 10));
    buffer[4] = (char)('0' + ((int)parsed_val % 10));
    buffer[5] = (char)('0' + (int)cmp);
    buffer[6] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
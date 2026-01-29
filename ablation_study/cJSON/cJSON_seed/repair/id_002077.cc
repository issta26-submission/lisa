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
//<ID> 2077
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON *count_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "count", count_item);

    // step 2: Configure
    char raw_json[] = " { \"note\" : \"hello\" , \"value\": 3.14 } ";
    cJSON_Minify(raw_json);
    cJSON *parsed = cJSON_Parse(raw_json);
    cJSON *note = cJSON_GetObjectItem(parsed, "note");
    cJSON *value = cJSON_GetObjectItem(parsed, "value");

    // step 3: Operate and Validate
    const char *note_str = cJSON_GetStringValue(note);
    double value_num = cJSON_GetNumberValue(value);
    cJSON *note_dup = cJSON_Duplicate(note, 1);
    cJSON_AddItemToObject(root, "note", note_dup);
    cJSON *value_dup = cJSON_CreateNumber(value_num);
    cJSON_AddItemToObject(root, "parsed_value", value_dup);
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, dup_root, 1);
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    const char *got_name = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    double got_count = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    buffer[0] = printed[0];
    buffer[1] = got_name[0];
    buffer[2] = (char)('0' + ((int)got_count % 10));
    buffer[3] = note_str[0];
    buffer[4] = (char)('0' + (equal ? 1 : 0));
    buffer[5] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_Delete(dup_root);
    // API sequence test completed successfully
    return 66;
}
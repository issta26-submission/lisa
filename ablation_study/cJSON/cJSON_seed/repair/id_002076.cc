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
//<ID> 2076
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
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta, "active", active);
    const char raw_json[] = "{\"name\":\"example_name\",\"count\":42}";
    cJSON *parsed = cJSON_Parse(raw_json);
    cJSON *parsed_name = cJSON_GetObjectItem(parsed, "name");
    cJSON *parsed_count = cJSON_GetObjectItem(parsed, "count");

    // step 3: Operate and Validate
    char *name_str = cJSON_GetStringValue(parsed_name);
    double count_val = cJSON_GetNumberValue(parsed_count);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = name_str[0];
    buffer[2] = (char)('0' + ((int)count_val % 10));
    buffer[3] = (char)('0' + equal);
    buffer[4] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
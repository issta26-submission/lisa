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
//<ID> 2072
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("ExampleName");
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON *count_item = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(root, "count", count_item);
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(settings, "mode", cJSON_CreateString("fast"));
    cJSON_AddItemToObject(root, "settings", settings);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(printed);
    cJSON *parsed_name = cJSON_GetObjectItem(parsed, "name");
    const char *parsed_name_str = cJSON_GetStringValue(parsed_name);
    cJSON *parsed_count = cJSON_GetObjectItem(parsed, "count");
    double parsed_count_val = cJSON_GetNumberValue(parsed_count);

    // step 3: Operate and Validate
    cJSON_bool are_equal = cJSON_Compare(root, parsed, 1);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = parsed_name_str ? parsed_name_str[0] : ' ';
    buffer[2] = (char)('0' + ((int)parsed_count_val % 10));
    buffer[3] = (char)('0' + (are_equal ? 1 : 0));
    buffer[4] = '\0';
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
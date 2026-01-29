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
//<ID> 2069
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
    const char raw_json[] = "{\"rawnum\":123}";
    cJSON_AddRawToObject(root, "raw", raw_json);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *got_name = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(got_name);
    cJSON *got_count = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(got_count);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = name_str[0];
    buffer[2] = raw_json[1];
    buffer[3] = (char)('0' + ((int)count_val % 10));
    buffer[4] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
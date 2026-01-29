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
//<ID> 2067
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
    const char raw_json[] = "{\"rawflag\":true,\"rawvalue\":3.14}";
    cJSON_AddRawToObject(root, "metadata", raw_json);

    // step 2: Configure
    cJSON *tag_item = cJSON_CreateString("primary");
    cJSON_AddItemToObject(root, "tag", tag_item);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    const char *name_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    buffer[0] = printed[0];
    buffer[1] = name_str[0];
    buffer[2] = raw_json[2];
    buffer[3] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
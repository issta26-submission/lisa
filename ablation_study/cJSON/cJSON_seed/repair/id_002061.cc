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
//<ID> 2061
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *greeting = cJSON_CreateString("hello_world");
    cJSON_AddItemToObject(root, "greeting", greeting);

    // step 2: Configure
    cJSON *pi_value = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(root, "pi", pi_value);
    const char raw_json[] = "{\"embedded\":true,\"n\":42}";
    cJSON_AddRawToObject(root, "raw_payload", raw_json);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    const char *gstr = cJSON_GetStringValue(greeting);
    buffer[0] = printed[0];
    buffer[1] = gstr[0];
    buffer[2] = raw_json[1];
    buffer[3] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
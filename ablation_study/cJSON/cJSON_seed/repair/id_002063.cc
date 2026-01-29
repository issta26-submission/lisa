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
//<ID> 2063
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
    cJSON *answer = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON_AddItemToObject(root, "answer", answer);

    // step 2: Configure
    const char raw_json[] = "{\"rawnum\":123}";
    cJSON_AddRawToObject(root, "raw", raw_json);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *got_greeting = cJSON_GetObjectItem(root, "greeting");
    cJSON *got_answer = cJSON_GetObjectItem(root, "answer");
    const char *greet_str = cJSON_GetStringValue(got_greeting);
    double answer_val = cJSON_GetNumberValue(got_answer);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed ? printed[0] : '\0';
    buffer[1] = greet_str ? greet_str[0] : '\0';
    buffer[2] = raw_json[1];
    buffer[3] = (char)('0' + ((int)answer_val % 10));
    buffer[4] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
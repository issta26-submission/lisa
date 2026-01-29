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
//<ID> 2055
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_buf[] = " { \"greeting\" : \"hello\", \"num\": 42 } ";
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    cJSON *greet_item = cJSON_GetObjectItem(parsed, "greeting");
    const char *greet_str = cJSON_GetStringValue(greet_item);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "from_parsed", greet_str);
    cJSON *dup_greet = cJSON_Duplicate(greet_item, 1);
    cJSON_AddItemToObject(root, "duplicate_greeting", dup_greet);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag_false);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "version", 1.0);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_SetNumberHelper(cJSON_GetObjectItem(meta, "version"), 2.0);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed ? printed[0] : '\0';
    buffer[1] = greet_str ? greet_str[0] : '\0';
    buffer[2] = cJSON_GetStringValue(cJSON_GetObjectItem(root, "duplicate_greeting"))[0];
    buffer[3] = cJSON_GetStringValue(cJSON_GetObjectItem(root, "from_parsed"))[0];
    buffer[4] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
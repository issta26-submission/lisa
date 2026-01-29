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
//<ID> 2120
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *answer = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(config, "answer", answer);
    cJSON *added_greeting = cJSON_AddStringToObject(config, "greeting", "hello");
    cJSON *arr_str = cJSON_CreateString("elem0");
    cJSON_AddItemToArray(items, arr_str);

    // step 3: Operate and Validate
    cJSON *got_answer = cJSON_GetObjectItemCaseSensitive(config, "answer");
    double answer_val = cJSON_GetNumberValue(got_answer);
    cJSON_AddNumberToObject(root, "copied_answer", answer_val);
    cJSON *got_greeting = cJSON_GetObjectItemCaseSensitive(config, "greeting");
    const char *greeting = cJSON_GetStringValue(got_greeting);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = greeting[0];
    buffer[1] = (char)('0' + ((int)answer_val % 10));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
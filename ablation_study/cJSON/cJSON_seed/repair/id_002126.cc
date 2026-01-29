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
//<ID> 2126
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
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "list", list);
    cJSON *n_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(list, n_item);

    // step 2: Configure
    cJSON *s_item = cJSON_CreateString("hello_world");
    cJSON_bool ok_add_str_to_array = cJSON_AddItemToArray(list, s_item);
    cJSON *greeting = cJSON_AddStringToObject(config, "greeting", "hi_there");
    cJSON *pi_item = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(config, "pi", pi_item);

    // step 3: Operate and Validate
    cJSON *got_greeting = cJSON_GetObjectItemCaseSensitive(config, "greeting");
    cJSON *got_pi = cJSON_GetObjectItemCaseSensitive(config, "pi");
    double pi_val = cJSON_GetNumberValue(got_pi);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    const char *gstr = cJSON_GetStringValue(got_greeting);
    buffer[0] = gstr[0];
    buffer[1] = (char)('0' + ((int)pi_val % 10));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
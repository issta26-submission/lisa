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
//<ID> 199
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_pi = cJSON_CreateNumber(3.14159);
    cJSON *str_greet = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "pi", num_pi);
    cJSON_AddItemToObject(root, "greeting", str_greet);

    // step 2: Configure
    cJSON *replacement_pi = cJSON_CreateNumber(2.71828);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "pi", replacement_pi);

    // step 3: Operate and Validate
    cJSON *item_pi = cJSON_GetObjectItemCaseSensitive(root, "pi");
    double pi_value = cJSON_GetNumberValue(item_pi);
    cJSON *item_greeting = cJSON_GetObjectItemCaseSensitive(root, "greeting");
    cJSON_bool greeting_is_string = cJSON_IsString(item_greeting);
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_Minify(unformatted);
    void *tmp_buf = cJSON_malloc(64);
    memset(tmp_buf, 0, 64);
    cJSON *parsed = cJSON_Parse(unformatted);
    int summary = (int)pi_value + (int)greeting_is_string + root->type + parsed->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_free(tmp_buf);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
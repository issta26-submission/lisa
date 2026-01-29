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
//<ID> 1247
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"greeting\":\"hello\",\"count\":42}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *newstr = cJSON_CreateString("initial");
    char *set_ret = cJSON_SetValuestring(newstr, "updated");
    cJSON_bool newstr_is_string = cJSON_IsString(newstr);

    // step 2: Configure
    cJSON_AddItemToObject(root, "newkey", newstr);
    cJSON *greet = cJSON_GetObjectItem(root, "greeting");
    char *greet_val = cJSON_GetStringValue(greet);
    char *newkey_val = cJSON_GetStringValue(newstr);

    // step 3: Operate and Validate
    cJSON_SetValuestring(greet, "world");
    cJSON_bool greet_is_string = cJSON_IsString(greet);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    ((char *)scratch)[2] = set_ret ? set_ret[0] : 0;
    ((char *)scratch)[3] = greet_val ? greet_val[0] : 0;
    ((char *)scratch)[4] = newkey_val ? newkey_val[0] : 0;
    ((char *)scratch)[5] = (char)(newstr_is_string + greet_is_string);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
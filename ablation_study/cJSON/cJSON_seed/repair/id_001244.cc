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
//<ID> 1244
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *orig = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", orig);
    cJSON *parsed = cJSON_Parse("\"world\"");

    // step 2: Configure
    char *parsed_str = cJSON_GetStringValue(parsed);
    char *prev_valuestring = cJSON_SetValuestring(orig, parsed_str);
    cJSON_bool orig_is_string = cJSON_IsString(orig);
    cJSON_bool parsed_is_string = cJSON_IsString(parsed);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    (void)prev_valuestring;
    (void)orig_is_string;
    (void)parsed_is_string;
    (void)parsed_str;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
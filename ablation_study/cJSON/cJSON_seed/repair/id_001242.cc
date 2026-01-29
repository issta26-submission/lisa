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
//<ID> 1242
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(list, s1);
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(list, s2);

    // step 2: Configure
    char *updated = cJSON_SetValuestring(s2, "beta-updated");
    cJSON *title = cJSON_CreateString("example-root");
    cJSON_AddItemToObject(root, "title", title);
    const char *json_text = "{\"msg\":\"hello\",\"ok\":true}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 3: Operate and Validate
    cJSON_bool s1_is_str = cJSON_IsString(s1);
    cJSON_bool s2_is_str = cJSON_IsString(s2);
    cJSON *parsed_msg = cJSON_GetObjectItem(parsed, "msg");
    cJSON_bool parsed_msg_is_str = cJSON_IsString(parsed_msg);
    double score_val = (double)s1_is_str + (double)s2_is_str + (double)parsed_msg_is_str;
    cJSON *score = cJSON_CreateNumber(score_val);
    cJSON_AddItemToObject(root, "score", score);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = out ? out[1] : 0;
    ((char *)scratch)[2] = updated ? updated[0] : 0;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1240
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *s = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "s", s);
    cJSON *parsed = cJSON_Parse("{\"parsed\":\"ok\"}");
    cJSON *pval = cJSON_GetObjectItem(parsed, "parsed");

    // step 2: Configure
    cJSON_SetValuestring(s, "updated");
    cJSON_bool s_is_string = cJSON_IsString(s);
    cJSON_bool pval_is_string = cJSON_IsString(pval);

    // step 3: Operate and Validate
    double score = (double)s_is_string + (double)pval_is_string;
    cJSON *score_num = cJSON_CreateNumber(score);
    cJSON_AddItemToObject(root, "flags_sum", score_num);
    const char *pstr = cJSON_GetStringValue(pval);
    cJSON *dup_parsed = cJSON_CreateString(pstr);
    cJSON_AddItemToObject(root, "dup_parsed", dup_parsed);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
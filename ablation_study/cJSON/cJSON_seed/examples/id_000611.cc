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
//<ID> 611
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *num_pi = cJSON_CreateNumber(3.14159);
    cJSON *num_ans = cJSON_CreateNumber(42.0);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "pi", num_pi);
    cJSON_AddItemToObject(root, "answer", num_ans);
    cJSON_AddItemToObject(root, "flag", flag_false);
    cJSON *null_item = cJSON_AddNullToObject(root, "note");

    // step 2: Configure
    double pi_val = cJSON_GetNumberValue(num_pi);
    double ans_val = cJSON_GetNumberValue(num_ans);
    char summary[256];
    memset(summary, 0, sizeof(summary));
    sprintf(summary, "ver=%s;pi=%.5f;ans=%.0f;note=%p",
            version ? version : "null",
            pi_val,
            ans_val,
            (void*)null_item);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_pi = cJSON_GetObjectItem(parsed, "pi");
    cJSON *parsed_ans = cJSON_GetObjectItem(parsed, "answer");
    double parsed_pi_val = cJSON_GetNumberValue(parsed_pi);
    double parsed_ans_val = cJSON_GetNumberValue(parsed_ans);
    cJSON *parsed_flag = cJSON_GetObjectItem(parsed, "flag");
    cJSON_bool is_false = cJSON_IsFalse(parsed_flag);
    char status[512];
    memset(status, 0, sizeof(status));
    sprintf(status, "%s;parsed_pi=%.5f;parsed_ans=%.0f;is_false=%d",
            summary,
            parsed_pi_val,
            parsed_ans_val,
            (int)is_false);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
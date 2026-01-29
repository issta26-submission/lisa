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
//<ID> 613
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
    cJSON *num_item = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(root, "answer", num_item);
    cJSON_AddNullToObject(root, "nothing");
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag_false);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "pi_approx", 3.14);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 3: Operate & Validate
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_answer = cJSON_GetObjectItem(parsed, "answer");
    double answer_value = cJSON_GetNumberValue(parsed_answer);
    cJSON_bool flag_is_false = cJSON_IsFalse(cJSON_GetObjectItem(parsed, "flag"));
    cJSON *parsed_nothing = cJSON_GetObjectItem(parsed, "nothing");
    const char *ver_display = version ? version : "null";
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;answer=%.1f;flag_false=%d;nothing=%p", ver_display, answer_value, (int)flag_is_false, (void*)parsed_nothing);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 156
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *greet = cJSON_CreateString("hello");
    cJSON_AddItemToObjectCS(root, "greeting", greet);
    cJSON *salute = cJSON_CreateString("world");
    cJSON_AddItemToObject(root, "salute", salute);
    cJSON *answer = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", answer);

    // step 3: Operate & Validate
    cJSON *got_greet = cJSON_GetObjectItem(root, "greeting");
    const char *greet_val = cJSON_GetStringValue(got_greet);
    cJSON *greet_ref = cJSON_CreateStringReference(greet_val);
    cJSON_AddItemToObject(meta, "greet_ref", greet_ref);
    cJSON *got_answer = cJSON_GetObjectItem(root, "answer");
    double ans_val = cJSON_GetNumberValue(got_answer);
    double next = ans_val + 1.0;
    cJSON_AddNumberToObject(root, "next", next);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
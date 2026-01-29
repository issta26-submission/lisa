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
//<ID> 308
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *flag = cJSON_CreateBool(1);
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON *answer = cJSON_CreateNumber(42.0);
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    const cJSON *retrieved_child = NULL;
    const cJSON *retrieved_flag = NULL;
    const cJSON *retrieved_greeting = NULL;
    const cJSON *retrieved_answer = NULL;
    const char *gstr = NULL;
    double ans_val = 0.0;
    cJSON_bool flag_is_bool = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "flag", flag);
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON_AddItemToObject(root, "answer", answer);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    retrieved_child = cJSON_GetObjectItem(root, "child");
    retrieved_flag = cJSON_GetObjectItem(retrieved_child, "flag");
    flag_is_bool = cJSON_IsBool(retrieved_flag);
    retrieved_greeting = cJSON_GetObjectItem(root, "greeting");
    gstr = cJSON_GetStringValue(retrieved_greeting);
    retrieved_answer = cJSON_GetObjectItem(root, "answer");
    ans_val = cJSON_GetNumberValue(retrieved_answer);
    buffer[0] = (char)('0' + (int)flag_is_bool);
    buffer[1] = gstr[0];
    buffer[2] = (char)('A' + (((int)ans_val) % 26));
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
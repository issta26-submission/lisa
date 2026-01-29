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
//<ID> 1294
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flag = cJSON_CreateFalse();
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON *root2 = cJSON_CreateObject();
    cJSON_AddItemToObject(root2, "flag", cJSON_CreateFalse());
    cJSON_AddItemToObject(root2, "greeting", cJSON_CreateString("hello"));

    // step 2: Configure
    cJSON_bool equal = cJSON_Compare(root, root2, 1);
    cJSON *got_flag = cJSON_GetObjectItem(root, "flag");
    cJSON *got_greeting = cJSON_GetObjectItem(root, "greeting");

    // step 3: Operate and Validate
    cJSON_bool is_false = cJSON_IsFalse(got_flag);
    cJSON_bool is_bool = cJSON_IsBool(got_flag);
    char *greet_val = cJSON_GetStringValue(got_greeting);
    char *printed = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(16);
    memset(scratch, 0, 16);
    ((char *)scratch)[0] = (char)('0' + (char)equal);
    ((char *)scratch)[1] = (char)('0' + (char)is_false);
    ((char *)scratch)[2] = (char)('0' + (char)is_bool);
    ((char *)scratch)[3] = greet_val ? greet_val[0] : '\0';
    ((char *)scratch)[4] = printed ? printed[0] : '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(root2);
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1296
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *user = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("Alice");
    cJSON *verified = cJSON_CreateFalse();
    cJSON_AddItemToObject(user, "name", name_item);
    cJSON_AddItemToObject(user, "verified", verified);
    cJSON_AddItemToObject(root, "user", user);
    cJSON *scores = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateNumber(90.0);
    cJSON *s2 = cJSON_CreateNumber(85.5);
    cJSON_AddItemToArray(scores, s1);
    cJSON_AddItemToArray(scores, s2);
    cJSON_AddItemToObject(root, "scores", scores);

    // step 2: Configure
    cJSON *duplicate_user = cJSON_Duplicate(user, 1);
    cJSON_bool compare_result = cJSON_Compare(user, duplicate_user, 1);
    cJSON_bool is_bool_flag = cJSON_IsBool(verified);
    cJSON_bool is_false_flag = cJSON_IsFalse(verified);

    // step 3: Operate and Validate
    char *name_val = cJSON_GetStringValue(name_item);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    ((char *)scratch)[2] = (name_val && name_val[0]) ? name_val[0] : 0;
    (void)compare_result;
    (void)is_bool_flag;
    (void)is_false_flag;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(duplicate_user);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
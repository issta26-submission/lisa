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
//<ID> 1293
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
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "active", flag);

    // step 2: Configure
    cJSON *scores = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateNumber(10.0);
    cJSON *s2 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(scores, s1);
    cJSON_AddItemToArray(scores, s2);
    cJSON_AddItemToObject(root, "scores", scores);
    cJSON *dup = cJSON_Duplicate(root, 1);

    // step 3: Operate and Validate
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    cJSON *name_item = cJSON_GetObjectItem(meta_item, "name");
    char *name_val = cJSON_GetStringValue(name_item);
    cJSON_bool is_bool = cJSON_IsBool(flag);
    cJSON_bool is_false = cJSON_IsFalse(flag);
    cJSON_bool are_equal = cJSON_Compare(root, dup, 1);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out ? out[0] : '\0';
    ((char *)scratch)[1] = out ? out[1] : '\0';
    ((char *)scratch)[2] = name_val ? name_val[0] : '\0';
    ((char *)scratch)[3] = (char)('0' + (is_bool ? 1 : 0));
    ((char *)scratch)[4] = (char)('0' + (is_false ? 1 : 0));
    ((char *)scratch)[5] = (char)('0' + (are_equal ? 1 : 0));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
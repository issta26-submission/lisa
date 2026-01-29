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
//<ID> 136
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *words[] = { "alpha", "beta", "gamma" };
    cJSON *str_array = cJSON_CreateStringArray(words, 3);
    float floats[] = { 1.5f, 2.5f, 3.5f };
    cJSON *float_array = cJSON_CreateFloatArray(floats, 3);
    cJSON_AddItemToObject(root, "words", str_array);
    cJSON_AddItemToObject(root, "values", float_array);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "owner", "tester");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "active", cJSON_CreateTrue());
    cJSON_AddItemToObject(root, "count", cJSON_CreateNumber(3.0));

    // step 2: Configure
    cJSON *float_dup = cJSON_Duplicate(float_array, 1);
    cJSON *str_dup = cJSON_Duplicate(str_array, 1);
    cJSON_AddItemToObject(root, "words_copy", str_dup);

    // step 3: Operate and Validate
    cJSON_bool same = cJSON_Compare(float_array, float_dup, 1);
    char *unformatted = cJSON_PrintUnformatted(root);
    char buf[64];
    memset(buf, 0, sizeof(buf));
    int summary = (int)same + (unformatted ? (int)unformatted[0] : 0) + cJSON_GetArraySize(str_array);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(float_dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
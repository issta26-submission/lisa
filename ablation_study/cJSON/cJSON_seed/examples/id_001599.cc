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
//<ID> 1599
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *words[3] = { "alpha", "beta", "gamma" };
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "words", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateString(words[0]));
    cJSON_AddItemToArray(arr, cJSON_CreateString(words[1]));
    cJSON_AddItemToArray(arr, cJSON_CreateString(words[2]));
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "is_false_flag", flag);

    // step 2: Configure
    int count = cJSON_GetArraySize(arr);
    cJSON *count_num = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(root, "words_count", count_num);
    cJSON *flag_false_num = cJSON_CreateNumber((double)cJSON_IsFalse(flag));
    cJSON_AddItemToObject(root, "flag_is_false_numeric", flag_false_num);

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
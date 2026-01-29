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
//<ID> 1593
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
    cJSON *w0 = cJSON_CreateString(words[0]);
    cJSON *w1 = cJSON_CreateString(words[1]);
    cJSON *w2 = cJSON_CreateString(words[2]);
    cJSON_AddItemToArray(arr, w0);
    cJSON_AddItemToArray(arr, w1);
    cJSON_AddItemToArray(arr, w2);
    cJSON_AddItemToObject(root, "words", arr);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    cJSON *copy = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "words_copy", copy);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    const char *first_val = cJSON_GetStringValue(first);
    cJSON *first_label = cJSON_CreateString(first_val);
    cJSON_AddItemToObject(root, "first_word", first_label);

    // step 3: Operate & Validate
    int size = cJSON_GetArraySize(arr);
    cJSON *size_num = cJSON_CreateNumber((double)size);
    cJSON_AddItemToObject(root, "size", size_num);
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    cJSON_bool flag_is_false = cJSON_IsFalse(flag_item);
    cJSON *flag_false_num = cJSON_CreateNumber((double)flag_is_false);
    cJSON_AddItemToObject(root, "flag_is_false", flag_false_num);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
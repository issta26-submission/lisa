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
//<ID> 1372
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *num42 = cJSON_CreateNumber(42.0);
    cJSON *greet = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "answer", num42);
    cJSON_AddItemToObject(root, "greet", greet);
    int nums_vals[3] = {1, 2, 3};
    cJSON *numbers = cJSON_CreateIntArray(nums_vals, 3);
    cJSON_AddItemToObject(root, "numbers", numbers);

    // step 2: Configure
    cJSON *extras = cJSON_CreateArray();
    cJSON_AddItemToArray(extras, cJSON_CreateNumber(7.5));
    cJSON_AddItemToArray(extras, cJSON_CreateNumber(8.25));
    cJSON_AddItemToObject(root, "extras", extras);

    // step 3: Operate
    cJSON_bool has_answer = cJSON_HasObjectItem(root, "answer");
    cJSON_AddItemToObject(root, "has_answer_flag", cJSON_CreateBool(has_answer));
    cJSON *dup_numbers = cJSON_Duplicate(numbers, 1);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "child", dup_numbers);
    char *printed = cJSON_PrintBuffered(root, 256, 1);

    // step 4: Validate & Cleanup
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 351
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *num_a = cJSON_CreateNumber(1.5);
    cJSON *num_b = cJSON_CreateNumber(2.5);
    cJSON *num_ins = cJSON_CreateNumber(3.5);
    cJSON *label = cJSON_CreateString("example");
    cJSON *flag_false = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToArray(items, num_a);
    cJSON_AddItemToArray(items, num_b);
    cJSON_InsertItemInArray(items, 1, num_ins);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToObject(root, "label", label);
    cJSON_AddItemToObject(root, "flag", flag_false);

    // step 3: Operate and Validate
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(items, 0));
    double second_val = cJSON_GetNumberValue(cJSON_GetArrayItem(items, 1));
    double computed_sum = first_val + second_val;
    cJSON_AddNumberToObject(root, "sum", computed_sum);
    cJSON_bool flag_was_false = cJSON_IsFalse(flag_false);
    cJSON_AddNumberToObject(root, "flag_is_false", (double)flag_was_false);
    char *printed = cJSON_Print(root);
    size_t printed_len = strlen(printed);
    char *mutable_buf = (char *)cJSON_malloc(printed_len + 1);
    memset(mutable_buf, 0, printed_len + 1);
    strcpy(mutable_buf, printed);
    cJSON_Minify(mutable_buf);
    cJSON_free(printed);
    cJSON_free(mutable_buf);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
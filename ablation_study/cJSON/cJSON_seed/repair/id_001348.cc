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
//<ID> 1348
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int numbers[3] = {10, 20, 30};
    cJSON *int_array = cJSON_CreateIntArray(numbers, 3);
    cJSON *label_ref = cJSON_CreateStringReference("detached_label");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *others = cJSON_CreateArray();

    // step 2: Configure
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON_AddItemToObject(root, "label", label_ref);
    cJSON_AddItemToObject(root, "flag", false_item);
    cJSON_AddItemToObject(root, "others", others);
    cJSON *detached_label = cJSON_DetachItemViaPointer(root, label_ref);
    cJSON_AddItemToArray(others, detached_label);
    cJSON *extra_num = cJSON_CreateNumber(99.0);
    cJSON_AddItemToArray(others, extra_num);

    // step 3: Operate and Validate
    int size_ints = cJSON_GetArraySize(int_array);
    int size_others = cJSON_GetArraySize(others);
    cJSON *first_int = cJSON_GetArrayItem(int_array, 0);
    double first_val = cJSON_GetNumberValue(first_int);
    const char *first_label = cJSON_GetStringValue(cJSON_GetArrayItem(others, 0));
    cJSON_bool flag_was_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "flag"));
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(80);
    memset(scratch, 0, 80);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + (size_ints % 10));
    ((char *)scratch)[2] = (char)('0' + (size_others % 10));
    ((char *)scratch)[3] = (char)('0' + (((int)first_val) % 10));
    ((char *)scratch)[4] = first_label ? first_label[0] : 0;
    ((char *)scratch)[5] = (char)('0' + (flag_was_false ? 1 : 0));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
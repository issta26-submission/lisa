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
//<ID> 1582
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *words[3];
    words[0] = "alpha";
    words[1] = "beta";
    words[2] = "gamma";
    cJSON *arr = cJSON_CreateStringArray(words, 3);
    cJSON_AddItemToObject(root, "strings", arr);

    // step 2: Configure
    cJSON *extra_str = cJSON_CreateString("delta");
    cJSON_AddItemReferenceToArray(arr, extra_str);
    cJSON *arr_copy = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "strings_copy", arr_copy);

    // step 3: Operate & Validate
    int size_before = cJSON_GetArraySize(arr);
    cJSON_DeleteItemFromArray(arr, 1);
    int size_after = cJSON_GetArraySize(arr);
    cJSON_bool has_strings = cJSON_HasObjectItem(root, "strings");
    cJSON_bool has_copy = cJSON_HasObjectItem(root, "strings_copy");
    double computed = (double)(size_before + size_after + (int)has_strings + (int)has_copy);
    cJSON_AddNumberToObject(root, "computed", computed);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
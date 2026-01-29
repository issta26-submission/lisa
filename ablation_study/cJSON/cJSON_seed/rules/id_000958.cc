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
//<ID> 958
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *s0 = NULL;
    cJSON *s1 = NULL;
    cJSON *s2 = NULL;
    cJSON *retrieved_arr = NULL;
    cJSON *retrieved_item = NULL;
    cJSON *dup = NULL;
    const char *str0 = "alpha";
    const char *str1 = "beta";
    const char *str2 = "gamma";
    const char *retrieved_str = NULL;
    char *printed_root = NULL;
    char *printed_dup = NULL;
    char *scratch = NULL;
    int scratch_len = 16;
    cJSON_bool cmp_equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "list");
    s0 = cJSON_CreateString(str0);
    s1 = cJSON_CreateString(str1);
    s2 = cJSON_CreateString(str2);
    cJSON_InsertItemInArray(arr, 0, s0);
    cJSON_InsertItemInArray(arr, 1, s1);
    cJSON_InsertItemInArray(arr, 1, s2);

    // step 3: Operate / Validate
    printed_root = cJSON_Print(root);
    dup = cJSON_Duplicate(root, 1);
    printed_dup = cJSON_Print(dup);
    retrieved_arr = cJSON_GetObjectItem(root, "list");
    retrieved_item = cJSON_GetArrayItem(retrieved_arr, 1);
    retrieved_str = cJSON_GetStringValue(retrieved_item);
    cmp_equal = cJSON_Compare(root, dup, 1);
    scratch = (char *)cJSON_malloc((size_t)scratch_len);
    memset(scratch, 0, (size_t)scratch_len);
    scratch[0] = retrieved_str ? retrieved_str[0] : 'X';
    scratch[1] = (char)('0' + (int)cmp_equal);

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_dup);
    cJSON_free(scratch);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
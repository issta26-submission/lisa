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
//<ID> 952
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
    cJSON *dup_item = NULL;
    const char *value_str = NULL;
    char *scratch = NULL;
    cJSON_bool cmp_result = 0;
    int scratch_len = 16;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "items");
    s0 = cJSON_CreateString("alpha");
    s2 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(arr, s0);
    cJSON_AddItemToArray(arr, s2);
    s1 = cJSON_CreateString("beta");
    cJSON_InsertItemInArray(arr, 1, s1);

    // step 3: Operate / Validate
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    retrieved_item = cJSON_GetArrayItem(retrieved_arr, 1);
    value_str = cJSON_GetStringValue(retrieved_item);
    dup_item = cJSON_Duplicate(retrieved_item, 1);
    cmp_result = cJSON_Compare(retrieved_item, dup_item, 1);
    scratch = (char *)cJSON_malloc((size_t)scratch_len);
    memset(scratch, 0, (size_t)scratch_len);
    scratch[0] = value_str[0];
    scratch[1] = (char)('0' + (int)cmp_result);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(dup_item);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 950
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
    cJSON *s1 = NULL;
    cJSON *s2 = NULL;
    cJSON *retrieved_arr = NULL;
    cJSON *first_element = NULL;
    cJSON *dup_first = NULL;
    cJSON_bool cmp_result = 0;
    const char *first_str = NULL;
    char *scratch = NULL;
    const int scratch_len = 16;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "list");
    s1 = cJSON_CreateString("first");
    s2 = cJSON_CreateString("inserted");
    cJSON_AddItemToArray(arr, s1);
    cJSON_InsertItemInArray(arr, 0, s2);

    // step 3: Operate / Validate
    retrieved_arr = cJSON_GetObjectItem(root, "list");
    first_element = cJSON_GetArrayItem(retrieved_arr, 0);
    dup_first = cJSON_Duplicate(first_element, 1);
    cmp_result = cJSON_Compare(first_element, dup_first, 1);
    first_str = cJSON_GetStringValue(first_element);
    scratch = (char *)cJSON_malloc((size_t)scratch_len);
    memset(scratch, 0, (size_t)scratch_len);
    scratch[0] = first_str ? first_str[0] : '\0';
    scratch[1] = (char)('0' + (int)cmp_result);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(dup_first);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 953
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
    cJSON *retrieved = NULL;
    cJSON *first_item = NULL;
    cJSON *dup_root = NULL;
    const char *first_str = NULL;
    char *scratch = NULL;
    int scratch_len = 32;
    cJSON_bool insert_res1 = 0;
    cJSON_bool insert_res2 = 0;
    cJSON_bool cmp_res = 0;
    const char *text1 = "alpha";
    const char *text2 = "beta";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "list");
    s1 = cJSON_CreateString(text1);
    s2 = cJSON_CreateString(text2);
    insert_res1 = cJSON_InsertItemInArray(arr, 0, s1);
    insert_res2 = cJSON_InsertItemInArray(arr, 1, s2);

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItem(root, "list");
    first_item = cJSON_GetArrayItem(retrieved, 0);
    first_str = cJSON_GetStringValue(first_item);
    dup_root = cJSON_Duplicate(root, 1);
    cmp_res = cJSON_Compare(root, dup_root, 1);
    scratch = (char *)cJSON_malloc((size_t)scratch_len);
    memset(scratch, 0, (size_t)scratch_len);
    scratch[0] = first_str[0];
    scratch[1] = (char)('0' + (int)insert_res1);
    scratch[2] = (char)('0' + (int)insert_res2);
    scratch[3] = (char)('0' + (int)cmp_res);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
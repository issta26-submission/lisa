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
//<ID> 959
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
    cJSON *s3 = NULL;
    cJSON *retrieved_arr = NULL;
    cJSON *first_item = NULL;
    char *printed = NULL;
    char *scratch = NULL;
    const char *first_str = NULL;
    cJSON_bool cmp_res = 0;
    int scratch_len = 8;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "letters");
    s1 = cJSON_CreateString("alpha");
    s2 = cJSON_CreateString("beta");
    s3 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s3);
    cJSON_InsertItemInArray(arr, 1, s2);

    // step 3: Operate / Validate
    retrieved_arr = cJSON_GetObjectItem(root, "letters");
    cmp_res = cJSON_Compare(arr, retrieved_arr, 1);
    first_item = cJSON_GetArrayItem(retrieved_arr, 0);
    first_str = cJSON_GetStringValue(first_item);
    printed = cJSON_Print(root);
    scratch = (char *)cJSON_malloc((size_t)scratch_len);
    memset(scratch, 0, (size_t)scratch_len);
    scratch[0] = first_str ? first_str[0] : '\0';
    scratch[1] = (char)('0' + (int)cmp_res);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
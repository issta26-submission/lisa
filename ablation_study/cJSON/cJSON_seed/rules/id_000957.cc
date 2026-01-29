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
//<ID> 957
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
    cJSON *first = NULL;
    cJSON *last = NULL;
    cJSON *middle = NULL;
    cJSON *retrieved_arr = NULL;
    cJSON *retrieved_item = NULL;
    const char *retrieved_str = NULL;
    char *printed = NULL;
    char *scratch = NULL;
    cJSON_bool compare_result = 0;
    int scratch_len = 32;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "items");
    first = cJSON_CreateString("alpha");
    last = cJSON_CreateString("omega");
    cJSON_AddItemToArray(arr, first);
    cJSON_AddItemToArray(arr, last);
    middle = cJSON_CreateString("beta");
    cJSON_InsertItemInArray(arr, 1, middle);

    // step 3: Operate / Validate
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    retrieved_item = cJSON_GetArrayItem(retrieved_arr, 1);
    retrieved_str = cJSON_GetStringValue(retrieved_item);
    compare_result = cJSON_Compare(middle, retrieved_item, 1);
    printed = cJSON_PrintUnformatted(root);
    scratch = (char *)cJSON_malloc((size_t)scratch_len);
    memset(scratch, 0, (size_t)scratch_len);
    scratch[0] = retrieved_str ? retrieved_str[0] : '\0';
    scratch[1] = (char)('0' + (int)compare_result);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
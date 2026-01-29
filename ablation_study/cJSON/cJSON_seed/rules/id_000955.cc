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
//<ID> 955
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
    cJSON *item_first = NULL;
    cJSON *item_second = NULL;
    cJSON *insert_item = NULL;
    cJSON *retrieved_arr = NULL;
    cJSON *retrieved_item = NULL;
    char *printed = NULL;
    char *scratch = NULL;
    const char *retrieved_str = NULL;
    cJSON_bool cmp_result = 0;
    int scratch_len = 16;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item_first = cJSON_CreateString("first");
    item_second = cJSON_CreateString("second");
    cJSON_AddItemToArray(arr, item_first);
    cJSON_AddItemToArray(arr, item_second);
    cJSON_AddItemToObject(root, "list", arr);
    insert_item = cJSON_CreateString("middle");
    cJSON_InsertItemInArray(arr, 1, insert_item);

    // step 3: Operate / Validate
    retrieved_arr = cJSON_GetObjectItem(root, "list");
    retrieved_item = cJSON_GetArrayItem(retrieved_arr, 1);
    retrieved_str = cJSON_GetStringValue(retrieved_item);
    cmp_result = cJSON_Compare(retrieved_item, insert_item, 1);
    printed = cJSON_PrintUnformatted(root);
    scratch = (char *)cJSON_malloc((size_t)scratch_len);
    memset(scratch, 0, (size_t)scratch_len);
    scratch[0] = retrieved_str ? retrieved_str[0] : 'X';
    scratch[1] = (char)('0' + (int)cmp_result);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
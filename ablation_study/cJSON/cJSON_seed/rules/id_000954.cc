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
//<ID> 954
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
    cJSON *item_at1 = NULL;
    cJSON *dup_item = NULL;
    char *retrieved_str = NULL;
    char *printed = NULL;
    char *scratch = NULL;
    cJSON_bool inserted = 0;
    cJSON_bool equal = 0;
    int scratch_len = 16;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "items");
    s1 = cJSON_CreateString("alpha");
    s2 = cJSON_CreateString("beta");
    s3 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s3);
    inserted = cJSON_InsertItemInArray(arr, 1, s2);

    // step 3: Operate / Validate
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    item_at1 = cJSON_GetArrayItem(retrieved_arr, 1);
    retrieved_str = cJSON_GetStringValue(item_at1);
    dup_item = cJSON_Duplicate(item_at1, 1);
    equal = cJSON_Compare(item_at1, dup_item, 1);
    printed = cJSON_Print(root);
    scratch = (char *)cJSON_malloc((size_t)scratch_len);
    memset(scratch, 0, (size_t)scratch_len);
    scratch[0] = retrieved_str[0];
    scratch[1] = (char)('0' + (int)equal);
    scratch[2] = (char)('0' + (int)inserted);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(dup_item);

    // API sequence test completed successfully
    return 66;
}
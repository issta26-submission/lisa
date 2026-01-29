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
//<ID> 341
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *str_a = "alpha";
    const char *str_b = "beta";
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *item_a = cJSON_CreateString(str_a);
    cJSON *item_b = cJSON_CreateString(str_b);
    cJSON *dup_b = cJSON_Duplicate(item_b, 1);
    cJSON *first_item = NULL;
    cJSON *third_item = NULL;
    cJSON_bool first_is_string = 0;
    cJSON_bool insert_ok = 0;
    cJSON_bool compare_ref_with_orig = 0;
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "list", array);
    cJSON_AddItemToArray(array, item_b);
    insert_ok = cJSON_InsertItemInArray(array, 0, item_a);
    cJSON_AddItemReferenceToArray(array, item_b);

    // step 3: Operate / Validate
    first_item = cJSON_GetArrayItem(array, 0);
    first_is_string = cJSON_IsString(first_item);
    third_item = cJSON_GetArrayItem(array, 2);
    compare_ref_with_orig = cJSON_Compare(third_item, item_b, 1);
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + (int)first_is_string);
    buffer[2] = (char)('0' + (int)compare_ref_with_orig);
    (void)insert_ok;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(dup_b);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
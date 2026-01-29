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
//<ID> 709
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);

    // step 2: Configure
    cJSON *s_insert = cJSON_CreateString("beta");
    cJSON_InsertItemInArray(items, 1, s_insert);

    // step 3: Operate and Validate
    cJSON *items_ref = cJSON_GetObjectItemCaseSensitive(root, "items");
    cJSON *meta_ref = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON *middle = cJSON_GetArrayItem(items_ref, 1);
    cJSON_bool middle_is_string = cJSON_IsString(middle);
    char *middle_val = cJSON_GetStringValue(middle);
    double first_char_code = 0.0;
    first_char_code = middle_val ? (double)(unsigned char)middle_val[0] : 0.0;
    cJSON_AddNumberToObject(meta_ref, "insert_first_char_code", first_char_code);
    int total = cJSON_GetArraySize(items_ref);
    cJSON_AddNumberToObject(root, "count", (double)total);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
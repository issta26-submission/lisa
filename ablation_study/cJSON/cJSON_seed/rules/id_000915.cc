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
//<ID> 915
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *items = NULL;
    cJSON *str_ref1 = NULL;
    cJSON *str_ref2 = NULL;
    cJSON *meta = NULL;
    cJSON *retrieved_items = NULL;
    int items_size = 0;
    char *scratch = NULL;
    const size_t scratch_len = 64;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    str_ref1 = cJSON_CreateStringReference("alpha");
    cJSON_AddItemToArray(items, str_ref1);
    str_ref2 = cJSON_CreateStringReference("beta");
    cJSON_AddItemToArray(items, str_ref2);
    scratch = (char *)cJSON_malloc(scratch_len);
    memset(scratch, 0, scratch_len);

    // step 3: Operate / Validate
    retrieved_items = cJSON_GetObjectItemCaseSensitive(root, "items");
    items_size = cJSON_GetArraySize(retrieved_items);
    scratch[0] = (char)('0' + (items_size % 10));
    cJSON_DeleteItemFromObjectCaseSensitive(root, "meta");

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)str_ref1;
    (void)str_ref2;
    (void)retrieved_items;
    (void)items_size;
    return 66;
}
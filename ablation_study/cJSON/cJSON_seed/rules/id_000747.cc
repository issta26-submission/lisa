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
//<ID> 747
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *orig_str = cJSON_CreateString("original_value");
    cJSON *got_item = NULL;
    cJSON *replacement = NULL;
    char *printed = NULL;
    cJSON_bool cmp_result = 0;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "mykey", orig_str);

    // step 3: Operate / Validate
    got_item = cJSON_GetObjectItem(root, "mykey");
    replacement = cJSON_CreateString("new_value");
    cmp_result = cJSON_Compare(got_item, replacement, 0);
    replaced = cJSON_ReplaceItemViaPointer(root, got_item, replacement);
    printed = cJSON_Print(root);
    memcpy(scratch, printed, strlen(printed) + 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
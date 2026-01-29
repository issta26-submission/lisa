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
//<ID> 721
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(128);
    memset(scratch, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *first_true = cJSON_CreateTrue();
    cJSON *replacement_true = NULL;
    cJSON *pointer_replacement = NULL;
    cJSON *got_flag = NULL;
    char *printed = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "flag", first_true);
    replacement_true = cJSON_CreateTrue();
    cJSON_ReplaceItemInObjectCaseSensitive(root, "flag", replacement_true);

    // step 3: Operate / Validate
    got_flag = cJSON_GetObjectItemCaseSensitive(root, "flag");
    pointer_replacement = cJSON_CreateTrue();
    cJSON_ReplaceItemViaPointer(root, got_flag, pointer_replacement);
    printed = cJSON_PrintUnformatted(root);
    memcpy(scratch, printed ? printed : "", printed ? strlen(printed) : 0);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 729
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
    cJSON *root = NULL;
    cJSON *initial_true = NULL;
    cJSON *replacement1 = NULL;
    cJSON *replacement2 = NULL;
    cJSON *got_flag = NULL;
    cJSON_bool added = 0;
    cJSON_bool replaced1 = 0;
    cJSON_bool replaced2 = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    initial_true = cJSON_CreateTrue();
    added = cJSON_AddItemToObject(root, "flag", initial_true);
    replacement1 = cJSON_CreateTrue();
    replaced1 = cJSON_ReplaceItemInObjectCaseSensitive(root, "flag", replacement1);

    // step 3: Operate / Validate
    got_flag = cJSON_GetObjectItem(root, "flag");
    replacement2 = cJSON_CreateTrue();
    replaced2 = cJSON_ReplaceItemViaPointer(root, got_flag, replacement2);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 116
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(100.0);
    cJSON *label_old = cJSON_CreateString("old_value");
    cJSON *label_new = cJSON_CreateString("new_value");
    cJSON *got_label = NULL;
    char *printed_before = NULL;
    char *printed_after = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "count", num);
    cJSON_AddItemToObject(root, "label", label_old);

    // step 3: Operate / Validate
    printed_before = cJSON_PrintUnformatted(root);
    got_label = cJSON_GetObjectItem(root, "label");
    cJSON_ReplaceItemViaPointer(root, got_label, label_new);
    printed_after = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed_before);
    cJSON_free(printed_after);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 868
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(3.14159);
    cJSON *label_ref = cJSON_CreateStringReference("const_label");

    // step 2: Configure
    cJSON_bool add_num_ok = cJSON_AddItemToObjectCS(root, "pi", num_item);
    cJSON_bool add_label_ok = cJSON_AddItemToObjectCS(root, "label", label_ref);

    // step 3: Operate & Validate
    double updated = cJSON_SetNumberHelper(num_item, 2.71828);
    char *snapshot = cJSON_PrintUnformatted(root);
    (void)updated;
    (void)add_num_ok;
    (void)add_label_ok;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
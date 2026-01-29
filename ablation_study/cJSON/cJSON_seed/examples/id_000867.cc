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
//<ID> 867
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *str_ref = cJSON_CreateStringReference("example");

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "value", num_item);
    cJSON_AddItemToObjectCS(root, "name_ref", str_ref);

    // step 3: Operate & Validate
    double updated = cJSON_SetNumberHelper(num_item, 3.14159);
    cJSON *double_item = cJSON_CreateNumber(updated * 2.0);
    cJSON_AddItemToObjectCS(root, "double_value", double_item);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
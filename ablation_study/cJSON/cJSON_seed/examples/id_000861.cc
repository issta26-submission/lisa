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
//<ID> 861
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *username = "FuzzUser";
    cJSON *num_item = cJSON_CreateNumber(3.1415);
    cJSON *name_ref = cJSON_CreateStringReference(username);

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "pi", num_item);
    cJSON_AddItemToObjectCS(root, "name", name_ref);

    // step 3: Operate & Validate
    double updated_value = cJSON_SetNumberHelper(num_item, 6.2830);
    cJSON *dup_num = cJSON_CreateNumber(updated_value);
    cJSON_AddItemToObjectCS(root, "double_pi", dup_num);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
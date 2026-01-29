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
//<ID> 1181
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num = NULL;
    cJSON *replacement = NULL;
    cJSON *got_num = NULL;
    cJSON *flag = NULL;
    cJSON *temp_orphan = NULL;
    double validated_value = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(321.0);
    cJSON_AddItemToObject(root, "value", num);
    flag = cJSON_AddTrueToObject(root, "ok");
    replacement = cJSON_CreateNumber(654.0);

    // step 3: Operate / Validate
    got_num = cJSON_GetObjectItem(root, "value");
    cJSON_ReplaceItemViaPointer(root, got_num, replacement);
    validated_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    (void)validated_value;
    temp_orphan = cJSON_CreateString("orphan");
    cJSON_Delete(temp_orphan);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
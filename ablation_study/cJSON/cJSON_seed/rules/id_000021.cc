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
//<ID> 21
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *num_initial = cJSON_CreateNumber(42.0);
    cJSON *num_replacement = cJSON_CreateNumber(84.0);
    cJSON *flag_item = cJSON_CreateTrue();
    cJSON *retr_flag = NULL;
    cJSON *retr_value = NULL;
    char *printed = NULL;
    int validation = 0;
    double value_read = 0.0;

    // step 2: Setup
    cJSON_AddItemToObject(root, "value", num_initial);
    cJSON_AddItemToObject(root, "flag", flag_item);

    // step 3: Operate / Validate
    cJSON_ReplaceItemViaPointer(root, num_initial, num_replacement);
    retr_flag = cJSON_GetObjectItem(root, "flag");
    retr_value = cJSON_GetObjectItem(root, "value");
    value_read = cJSON_GetNumberValue(retr_value);
    validation = (int)cJSON_IsTrue(retr_flag) + 2 * (int)(value_read == 84.0);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}